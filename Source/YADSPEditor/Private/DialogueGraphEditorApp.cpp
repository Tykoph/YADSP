#include "DialogueGraphEditorApp.h"
#include "DialogueSystemAppMode.h"
#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogueGraphSchema.h"
#include "DialogueSystemRuntimeGraph.h"
#include "DialogueNodeInfo.h"
#include "DialogueGraphNodeStart.h"
#include "DialogueGraphNodeEnd.h"

DEFINE_LOG_CATEGORY_STATIC(DialogueGraphEditorAppSub, Log, All)

void DialogueGraphEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void DialogueGraphEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	WorkingGraphAsset = Cast<UDialogueSystem>(InObject);
	WorkingGraphAsset->SetPreSaveListener([this](){OnWorkingGraphAssetPreSave();});

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

	AddApplicationMode(TEXT("DialogueGraphAppMode"), MakeShareable(new DialogueSystemAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("DialogueGraphAppMode"));

	UpdateGraphEditorFromWorkingAsset();
}

void DialogueGraphEditorApp::SetSelectedNodeDetailView(TSharedPtr<IDetailsView> SelectedNodeDetailView)
{
	SelectedNodeDetailViewPtr = SelectedNodeDetailView;
	SelectedNodeDetailViewPtr->OnFinishedChangingProperties().AddRaw(this, &DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated);
}

// Return the first UDialogueGraphNode in the selection set
UDialogueGraphNodeBase* DialogueGraphEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& SelectionSet)
{
	for (UObject* Obj : SelectionSet)
	{
		UDialogueGraphNodeBase* SelectedNode = Cast<UDialogueGraphNodeBase>(Obj);
		if (SelectedNode != nullptr)
		{
			return SelectedNode;
		}
	}

	return nullptr;
}

void DialogueGraphEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet)
{
	UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(SelectionSet);
	if (SelectedNode != nullptr)
	{
		SelectedNodeDetailViewPtr->SetObject(SelectedNode->GetNodeInfo());
	}
	else
	{
		SelectedNodeDetailViewPtr->SetObject(nullptr);
	}
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
	WorkingGraphAsset->SetPreSaveListener(nullptr);
	FAssetEditorToolkit::OnClose();
}

void DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event)
{
	if (WorkingGraphUiPtr != nullptr)
	{
		//get the node getting modified
		UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(WorkingGraphUiPtr->GetSelectedNodes());
		if (SelectedNode != nullptr)
		{
			SelectedNode->OnPropertiesChanged();
		}
		WorkingGraphUiPtr->NotifyGraphChanged();
	}
}

void DialogueGraphEditorApp::OnWorkingGraphAssetPreSave()
{
	UpdateWorkingAssetFromGraph();
	UE_LOG(DialogueGraphEditorAppSub, Log, TEXT("Working Graph pre save"));

}

void DialogueGraphEditorApp::UpdateWorkingAssetFromGraph()
{
	if (WorkingGraphAsset == nullptr || WorkingGraphEditor == nullptr)
	{
		UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("WorkingGraphEditor is null."));
		return;
	}

	UDialogueSystemRuntimeGraph* RuntimeGraph = NewObject<UDialogueSystemRuntimeGraph>(WorkingGraphAsset);
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

		UDialogueGraphNodeBase* UiGraphNode = Cast<UDialogueGraphNodeBase>(UiNode);

		if (UiGraphNode->GetNodeInfo() == nullptr)
		{
			UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("NodeInfo was null in UpdateWorkingAssetFromGraph."));
		}

		RuntimeNode->NodeInfo = DuplicateObject(UiGraphNode->GetNodeInfo(), RuntimeNode);
		RuntimeNode->NodeType = UiGraphNode->GetNodeType();

		if (RuntimeNode->NodeInfo == nullptr)
		{
			UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("RuntimeNode was null in UpdateWorkingAssetFromGraph."));
		}
		RuntimeGraph->Nodes.Add(RuntimeNode);
	}

	for (std::pair<FGuid, FGuid> Connection : Connections)
	{
		UDialogueRuntimeGraphPin* OutputPin = IdToPinMap[Connection.first];
		UDialogueRuntimeGraphPin* InputPin = IdToPinMap[Connection.second];

		OutputPin->Connection = InputPin;
	}

	UE_LOG(DialogueGraphEditorAppSub, Log, TEXT("Working Graph Updated."));
}

void DialogueGraphEditorApp::UpdateGraphEditorFromWorkingAsset()
{
	if (WorkingGraphEditor == nullptr)
	{
		UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("WorkingGraphEditor is null in UpdateGraphEditorFromWorkingAsset."));
		return;
	}

	if (WorkingGraphAsset->Graph == nullptr)
	{
		WorkingGraphAsset->Graph = NewObject<UDialogueSystemRuntimeGraph>(WorkingGraphAsset);
		UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("WorkingGraphAsset->Graph was null. Initialized a new graph."));
		WorkingGraphEditor->GetSchema()->CreateDefaultNodesForGraph(*WorkingGraphEditor);
	}

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;

	for (UDialogueRuntimeGraphNode* RuntimeNode : WorkingGraphAsset->Graph->Nodes)
	{
		UDialogueGraphNodeBase* NewNode = nullptr;
		if (RuntimeNode->NodeType == EDialogueNodeType::StartNode) {
			NewNode = NewObject<UDialogueGraphNodeStart>(WorkingGraphEditor);
		} else if (RuntimeNode->NodeType == EDialogueNodeType::DialogueNode) {
			NewNode = NewObject<UDialogueGraphNode>(WorkingGraphEditor);
		} else if (RuntimeNode->NodeType == EDialogueNodeType::EndNode) {
			NewNode = NewObject<UDialogueGraphNodeEnd>(WorkingGraphEditor);
		} else {
			UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("Unknown node type in UpdateGraphEditorFromWorkingAsset."));
		}

		NewNode->CreateNewGuid();
		NewNode->NodePosX = RuntimeNode->NodePosition.X;
		NewNode->NodePosY = RuntimeNode->NodePosition.Y;

		if (RuntimeNode->NodeInfo != nullptr) {
			NewNode->SetNodeInfo(DuplicateObject(RuntimeNode->NodeInfo, RuntimeNode));
		} else {
			NewNode->InitNodeInfo(NewNode);
			UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("NodeInfo was null in UpdateGraphEditorFromWorkingAsset."));
		}

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
