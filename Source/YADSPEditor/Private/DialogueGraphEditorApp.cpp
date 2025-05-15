// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueGraphEditorApp.h"
#include "DialogueSystemAppMode.h"
#include "DialogueSystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogueGraphSchema.h"
#include "DialogueGraphNodeStart.h"
#include "DialogueGraphNodeText.h"
#include "DialogueGraphNodeAction.h"
#include "DialogueGraphNodeAnimation.h"
#include "DialogueGraphNodeEnd.h"
#include "DialogueGraphNodeBase.h"

DEFINE_LOG_CATEGORY_STATIC(DialogueGraphEditorAppSub, Log, All)

void DialogueGraphEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void DialogueGraphEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	// initialize the working asset and graph editor
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

	// initialize the asset editor
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		TEXT("DialogueGraphEditorApp"),
		FTabManager::FLayout::NullLayout,
		true, // bCreateDefaultStandaloneMenu
		true, // bCreateDefaultToolbar
		ObjectsToEdit
	);

	// set the current mode to the DialogueGraphAppMode
	AddApplicationMode(TEXT("DialogueGraphAppMode"), MakeShareable(new DialogueSystemAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("DialogueGraphAppMode"));

	// update the graph editor from the working asset
	UpdateGraphEditorFromWorkingAsset();
}

void DialogueGraphEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingAsset->SetPreSaveListener(nullptr);
	FAssetEditorToolkit::OnClose();
}

// Called when the properties of the node detail view change
void DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event)
{
	if (WorkingGraphUiPtr != nullptr) {
		//get the node getting modified
		UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(WorkingGraphUiPtr->GetSelectedNodes());
		if (SelectedNode != nullptr) {
			// Call OnPropertiesChanged() on the selected node
			SelectedNode->OnPropertiesChanged();
		}

		// Notify the graph UI that the graph has changed
		WorkingGraphUiPtr->NotifyGraphChanged();
	}
}

void DialogueGraphEditorApp::OnWorkingGraphAssetPreSave()
{
	UpdateWorkingAssetFromGraph();
}

// Update the opened Dialogue Graph Asset with the current graph editor
// Called when opening a Dialogue Graph Asset
void DialogueGraphEditorApp::UpdateWorkingAssetFromGraph() const
{
	if (WorkingAsset == nullptr || WorkingGraphEditor == nullptr) { return; }

	// Create a new runtime graph with the current Dialogue Graph Asset
	UDialogueSystemRuntimeGraph* RuntimeGraph = NewObject<UDialogueSystemRuntimeGraph>(WorkingAsset);
	WorkingAsset->Graph = RuntimeGraph;

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UDialogueRuntimeGraphPin*> IdToPinMap;

	// Loop through all the nodes in the graph editor
	for (UEdGraphNode* UiNode : WorkingGraphEditor->Nodes) {
		UDialogueRuntimeGraphNode* RuntimeNode = NewObject<UDialogueRuntimeGraphNode>(RuntimeGraph);
		RuntimeNode->NodePosition = FVector2d(UiNode->NodePosX, UiNode->NodePosY);

		// Loop through all the pins in the node
		for (UEdGraphPin* UIPin : UiNode->Pins) {
			UDialogueRuntimeGraphPin* RuntimePin = NewObject<UDialogueRuntimeGraphPin>(RuntimeNode);
			RuntimePin->PinName = UIPin->PinName;
			RuntimePin->PinId = UIPin->PinId;
			RuntimePin->ParentNode = RuntimeNode;

			// If the pin is an output pin and has any connections, add the connection to the list
			if (UIPin->HasAnyConnections() && UIPin->Direction == EGPD_Output) {
				std::pair<FGuid, FGuid> Connection = std::make_pair(UIPin->PinId, UIPin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}

			// Add the pin to the map
			IdToPinMap.Add(UIPin->PinId, RuntimePin);

			// If the pin is an input pin, set it as the input pin on the runtime node
			if (UIPin->Direction == EGPD_Input) {
				RuntimeNode->InputPin = RuntimePin;
			}
			else {
				// If the pin is an output pin, add it to the output pins on the runtime node
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}

		// Duplicate the node info from the graph editor and add it to the runtime node
		UDialogueGraphNodeBase* UiGraphNode = Cast<UDialogueGraphNodeBase>(UiNode);
		RuntimeNode->NodeInfo = DuplicateObject(UiGraphNode->GetNodeInfo(), RuntimeNode);
		RuntimeNode->NodeType = UiGraphNode->GetNodeType();

		// Add the runtime node to the list of nodes on the runtime graph
		RuntimeGraph->Nodes.Add(RuntimeNode);
	}

	// Loop through all the connections and set the connected pin on the output pin
	for (std::pair<FGuid, FGuid> Connection : Connections) {
		UDialogueRuntimeGraphPin* OutputPin = IdToPinMap[Connection.first];
		UDialogueRuntimeGraphPin* InputPin = IdToPinMap[Connection.second];

		OutputPin->ConnectedPin = InputPin;
	}

	UE_LOG(DialogueGraphEditorAppSub, Log, TEXT("Working Graph Updated."));
}

// Update the graph editor with the current opened Dialogue Graph Asset
void DialogueGraphEditorApp::UpdateGraphEditorFromWorkingAsset() const
{
	// Check if the graph exists in the working asset
	if (WorkingAsset->Graph == nullptr) {
		// Create a new runtime graph if not present
		UDialogueSystemRuntimeGraph* RuntimeGraph = NewObject<UDialogueSystemRuntimeGraph>(WorkingAsset);
		// Create default nodes for the graph editor
		WorkingGraphEditor->GetSchema()->CreateDefaultNodesForGraph(*WorkingGraphEditor);
		return;
	}

	// Collections for managing connections and pin mappings
	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;

	// Iterate over each node in the runtime graph
	for (UDialogueRuntimeGraphNode* RuntimeNode : WorkingAsset->Graph->Nodes) {
		UDialogueGraphNodeBase* NewNode = nullptr;

		// Determine node type and create corresponding node object
		switch (RuntimeNode->NodeType) {
		case EDialogueNodeType::StartNode:
			NewNode = NewObject<UDialogueGraphNodeStart>(WorkingGraphEditor);
			break;
		case EDialogueNodeType::EndNode:
			NewNode = NewObject<UDialogueGraphNodeEnd>(WorkingGraphEditor);
			break;
		case EDialogueNodeType::TextNode:
			NewNode = NewObject<UDialogueGraphNodeText>(WorkingGraphEditor);
			break;
		case EDialogueNodeType::ActionNode:
			NewNode = NewObject<UDialogueGraphNodeAction>(WorkingGraphEditor);
			break;
		case EDialogueNodeType::AnimationNode:
			NewNode = NewObject<UDialogueGraphNodeAnimation>(WorkingGraphEditor);
			break;
		default:
			// Log an error for unknown node types
			UE_LOG(DialogueGraphEditorAppSub, Error, TEXT("Unknown node type in UpdateGraphEditorFromWorkingAsset."));
			break;
		}

		// Initialize node properties
		NewNode->CreateNewGuid();
		NewNode->NodePosX = RuntimeNode->NodePosition.X;
		NewNode->NodePosY = RuntimeNode->NodePosition.Y;

		// Handle node information duplication
		if (RuntimeNode->NodeInfo != nullptr) {
			NewNode->SetNodeInfo(DuplicateObject(RuntimeNode->NodeInfo, NewNode));
		}
		else {
			NewNode->InitNodeInfo(NewNode);
			UE_LOG(DialogueGraphEditorAppSub, Error,
			       TEXT("%ls->NodeInfo was null in UpdateGraphEditorFromWorkingAsset."),
			       *NewNode->GetNodeTitle(ENodeTitleType::FullTitle).ToString());
		}

		// Process input pin
		if (RuntimeNode->InputPin != nullptr) {
			UDialogueRuntimeGraphPin* InputPin = RuntimeNode->InputPin;
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Input, InputPin->PinName);
			UIPin->PinId = InputPin->PinId;

			// Add connection if the input pin is connected
			if (InputPin->ConnectedPin != nullptr) {
				Connections.Add(std::make_pair(InputPin->PinId, InputPin->ConnectedPin->PinId));
			}

			IdToPinMap.Add(InputPin->PinId, UIPin);
		}

		// Process output pins
		for (UDialogueRuntimeGraphPin* OutputPin : RuntimeNode->OutputPins) {
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Output, OutputPin->PinName);
			UIPin->PinId = OutputPin->PinId;

			// Add connection if the output pin is connected
			if (OutputPin->ConnectedPin != nullptr) {
				Connections.Add(std::make_pair(OutputPin->PinId, OutputPin->ConnectedPin->PinId));
			}

			IdToPinMap.Add(OutputPin->PinId, UIPin);
		}

		// Add the new node to the graph editor
		WorkingGraphEditor->AddNode(NewNode, true, true);
	}

	// Establish connections between pins based on the collected data
	for (std::pair<FGuid, FGuid> Connection : Connections) {
		UEdGraphPin* FromPin = IdToPinMap[Connection.first];
		UEdGraphPin* ToPin = IdToPinMap[Connection.second];

		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}
}

UDialogueGraphNodeBase* DialogueGraphEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& SelectionSet)
{
	for (UObject* Obj : SelectionSet) {
		UDialogueGraphNodeBase* SelectedNode = Cast<UDialogueGraphNodeBase>(Obj);
		if (SelectedNode != nullptr) {
			return SelectedNode;
		}
	}

	return nullptr;
}

void DialogueGraphEditorApp::SetSelectedNodeDetailView(const TSharedPtr<IDetailsView>& SelectedNodeDetailView)
{
	SelectedNodeDetailViewPtr = SelectedNodeDetailView;
	SelectedNodeDetailViewPtr->OnFinishedChangingProperties().AddRaw(
		this, &DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated);
}

void DialogueGraphEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet) const
{
	UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(SelectionSet);
	if (SelectedNode != nullptr) {
		SelectedNodeDetailViewPtr->SetObject(SelectedNode->GetNodeInfo());
	}
	else {
		SelectedNodeDetailViewPtr->SetObject(nullptr);
	}
}
