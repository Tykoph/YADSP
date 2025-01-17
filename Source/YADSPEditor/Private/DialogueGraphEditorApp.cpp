#include "DialogueGraphEditorApp.h"
#include "DialogueSystemAppMode.h"
#include "DialogueSystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogueGraphSchema.h"
#include "DialogueGraphNodeStart.h"
#include "DialogueGraphNodeEnd.h"
#include "DialogueGraphNodeText.h"
#include "DialogueGraphNodeBase.h"

DEFINE_LOG_CATEGORY_STATIC(DialogueGraphEditorAppSub, Log, All)

void DialogueGraphEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void DialogueGraphEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                        UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	WorkingAsset = Cast<UDialogueSystem>(InObject);
	WorkingAsset->SetPreSaveListener([this]() { OnWorkingGraphAssetPreSave(); });

	WorkingGraphEditor = FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
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

void DialogueGraphEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingAsset->SetPreSaveListener(nullptr);
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
}

void DialogueGraphEditorApp::UpdateWorkingAssetFromGraph()
{
	if (WorkingAsset == nullptr || WorkingGraphEditor == nullptr) { return; }

	UDialogueSystemRuntimeGraph* RuntimeGraph = NewObject<UDialogueSystemRuntimeGraph>(WorkingAsset);
	WorkingAsset->Graph = RuntimeGraph;

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
			RuntimePin->PinId = UIPin->PinId;
			RuntimePin->Parent = RuntimeNode;

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
		RuntimeNode->NodeInfo = DuplicateObject(UiGraphNode->GetNodeInfo(), RuntimeNode);
		RuntimeNode->NodeType = UiGraphNode->GetNodeType();

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
	if (WorkingAsset->Graph == nullptr)
	{
		UDialogueSystemRuntimeGraph* RuntimeGraph = NewObject<UDialogueSystemRuntimeGraph>(WorkingAsset);
		WorkingGraphEditor->GetSchema()->CreateDefaultNodesForGraph(*WorkingGraphEditor);
		return;
	}

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;

	for (UDialogueRuntimeGraphNode* RuntimeNode : WorkingAsset->Graph->Nodes)
	{
		UDialogueGraphNodeBase* NewNode = nullptr;
		switch (RuntimeNode->NodeType)
		{
		case EDialogueNodeType::StartNode:
			NewNode = NewObject<UDialogueGraphNodeStart>(WorkingGraphEditor);
			break;
		case EDialogueNodeType::EndNode:
			NewNode = NewObject<UDialogueGraphNodeEnd>(WorkingGraphEditor);
			break;
		case EDialogueNodeType::TextNode:
			NewNode = NewObject<UDialogueGraphNodeText>(WorkingGraphEditor);
			break;
		default:
			UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("Unknown node type in UpdateGraphEditorFromWorkingAsset."));
			break;
		}

		NewNode->CreateNewGuid();
		NewNode->NodePosX = RuntimeNode->NodePosition.X;
		NewNode->NodePosY = RuntimeNode->NodePosition.Y;

		if (RuntimeNode->NodeInfo != nullptr)
		{
			NewNode->SetNodeInfo(DuplicateObject(RuntimeNode->NodeInfo, NewNode));
		}
		else
		{
			NewNode->InitNodeInfo(NewNode);
			UE_LOG(DialogueGraphEditorAppSub, Error,
			       TEXT("%ls->NodeInfo was null in UpdateGraphEditorFromWorkingAsset."),
			       *NewNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
		}

		if (RuntimeNode->InputPin != nullptr)
		{
			UDialogueRuntimeGraphPin* InputPin = RuntimeNode->InputPin;
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Input, InputPin->PinName);
			UIPin->PinId = InputPin->PinId;

			if (InputPin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(InputPin->PinId, InputPin->Connection->PinId));
			}

			IdToPinMap.Add(InputPin->PinId, UIPin);
		}

		for (UDialogueRuntimeGraphPin* OutputPin : RuntimeNode->OutputPins)
		{
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Output, OutputPin->PinName);
			UIPin->PinId = OutputPin->PinId;

			if (OutputPin->Connection != nullptr)
			{
				Connections.Add(std::make_pair(OutputPin->PinId, OutputPin->Connection->PinId));
			}

			IdToPinMap.Add(OutputPin->PinId, UIPin);
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

void DialogueGraphEditorApp::SetSelectedNodeDetailView(TSharedPtr<IDetailsView> SelectedNodeDetailView)
{
	SelectedNodeDetailViewPtr = SelectedNodeDetailView;
	SelectedNodeDetailViewPtr->OnFinishedChangingProperties().AddRaw(
		this, &DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated);
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
