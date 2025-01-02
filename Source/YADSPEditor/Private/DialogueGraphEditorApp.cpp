#include "DialogueGraphEditorApp.h"
#include "DialogueGraphAppMode.h"
#include "CoreMinimal.h"
#include "DialogueGraph.h"
#include "DialogueGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogueGraphSchema.h"
#include "DialogueRuntimeGraph.h"


void DialogueGraphEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void DialogueGraphEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	WorkingGraphAsset = Cast<UDialogueGraph>(InObject);
	if (!WorkingGraphAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast InObject to UDialogueGraph!"));
		return;
	}

	WorkingGraphEditor = FBlueprintEditorUtils::CreateNewGraph(
		WorkingGraphAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UDialogueGraphSchema::StaticClass()
		);

	InitAssetEditor(
		Mode,
		InitToolkitHost,
		TEXT("DialogueGraphEditorApp"),
		FTabManager::FLayout::NullLayout,
		true, // bCreateDefaultStandaloneMenu
		true, // bCreateDefaultToolbar
		ObjectsToEdit
		);

	AddApplicationMode(TEXT("DialogueGraphAppMode"), MakeShareable(new DialogueGraphAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("DialogueGraphAppMode"));

	UpdateGraphEditorFromWorkingAsset();

	OnGraphChangedListenerHandle = WorkingGraphEditor->AddOnGraphChangedHandler(
		FOnGraphChanged::FDelegate::CreateRaw(this, &DialogueGraphEditorApp::OnGraphChanged));
}

void DialogueGraphEditorApp::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingFinished(Toolkit);
}

void DialogueGraphEditorApp::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
	FWorkflowCentricApplication::OnToolkitHostingStarted(Toolkit);
}

void DialogueGraphEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingGraphEditor->RemoveOnGraphChangedHandler(OnGraphChangedListenerHandle);

	UE_LOG(LogTemp, Log, TEXT("Working Graph Updated on close."));

	FAssetEditorToolkit::OnClose();
}

void DialogueGraphEditorApp::OnGraphChanged(const FEdGraphEditAction& Action)
{
	UpdateWorkingAssetFromGraph();
	UE_LOG(LogTemp, Log, TEXT("Working Graph Updated."));

}

void DialogueGraphEditorApp::UpdateWorkingAssetFromGraph()
{
	if (WorkingGraphAsset == nullptr || WorkingGraphEditor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("WorkingGraphEditor is null."));
		return;
	}

	UDialogueRuntimeGraph* RuntimeGraph = NewObject<UDialogueRuntimeGraph>(WorkingGraphAsset);
	WorkingGraphAsset->Graph = RuntimeGraph;

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UDialogueRuntimeGraphPin*> IdToPinMap;

	for (UEdGraphNode* UiNode : WorkingGraphEditor->Nodes)
	{
		UDialogueRuntimeGraphNode* RuntimeNode = NewObject<UDialogueRuntimeGraphNode>(RuntimeGraph);
		RuntimeNode->NodePosition = FVector2d(UiNode->NodePosX, UiNode->NodePosY);

		for (UEdGraphPin* UIPin : UiNode->Pins)
		{
			UDialogueRuntimeGraphPin* RuntimePin = NewObject<UDialogueRuntimeGraphPin>(RuntimeNode);
			RuntimePin->PinName = UIPin->PinName;
			RuntimePin->PinGuid = UIPin->PinId;

			if (UIPin->HasAnyConnections() && UIPin->Direction == EGPD_Output)
			{
				std::pair<FGuid, FGuid> Connection = std::make_pair(UIPin->PinId, UIPin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}

			IdToPinMap.Add(UIPin->PinId, RuntimePin);

			if (UIPin->Direction == EGPD_Input)
			{
				RuntimeNode->InputPin = RuntimePin;
			}
			else
			{
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}

		RuntimeGraph->Nodes.Add(RuntimeNode);
	}

	for (std::pair<FGuid, FGuid> Connection : Connections)
	{
		UDialogueRuntimeGraphPin* OutputPin = IdToPinMap[Connection.first];
		UDialogueRuntimeGraphPin* InputPin = IdToPinMap[Connection.second];

		OutputPin->Connection = InputPin;
	}
}

void DialogueGraphEditorApp::UpdateGraphEditorFromWorkingAsset()
{
	if (WorkingGraphAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WorkingGraphAsset is null!"));
		return;
	}

	if (WorkingGraphAsset->Graph == nullptr)
	{
		WorkingGraphAsset->Graph = NewObject<UDialogueRuntimeGraph>(WorkingGraphAsset);
		OnGraphChangedListenerHandle = WorkingGraphEditor->AddOnGraphChangedHandler(
			FOnGraphChanged::FDelegate::CreateRaw(this, &DialogueGraphEditorApp::OnGraphChanged));

		UE_LOG(LogTemp, Warning, TEXT("WorkingGraphAsset->Graph was null. Initialized a new graph."));
	}

	if (WorkingGraphAsset->Graph->Nodes.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WorkingGraphAsset->Graph->Nodes is empty."));
		return;
	}

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;

	for (UDialogueRuntimeGraphNode* RuntimeNode : WorkingGraphAsset->Graph->Nodes)
	{
		UDialogueGraphNode* NewNode = NewObject<UDialogueGraphNode>(WorkingGraphEditor);
		NewNode->NodePosX = RuntimeNode->NodePosition.X;
		NewNode->NodePosY = RuntimeNode->NodePosition.Y;

		if (RuntimeNode->InputPin != nullptr)
		{
			UDialogueRuntimeGraphPin* InputPin = RuntimeNode->InputPin;
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Input, InputPin->PinName);
			UIPin->PinId = InputPin->PinGuid;

			if (InputPin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(InputPin->PinGuid, InputPin->Connection->PinGuid));
			}

			IdToPinMap.Add(InputPin->PinGuid, UIPin);
		}

		for (UDialogueRuntimeGraphPin* OutputPin : RuntimeNode->OutputPins)
		{
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Output, OutputPin->PinName);
			UIPin->PinId = OutputPin->PinGuid;

			if (OutputPin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(OutputPin->PinGuid, OutputPin->Connection->PinGuid));
			}

			IdToPinMap.Add(OutputPin->PinGuid, UIPin);
		}

		WorkingGraphEditor->AddNode(NewNode, true, true);
	}

	for (std::pair<FGuid, FGuid> Connection : Connections)
	{
		UEdGraphPin* FromPin = IdToPinMap[Connection.first];
		UEdGraphPin* ToPin = IdToPinMap[Connection.second];

		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}
}
