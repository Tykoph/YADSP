// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Compiler/DialogueGraphCompiler.h"

#include "DialogueSystemRuntimeGraph.h"
#include "Nodes/DialogueGraphNodeGameAction.h"
#include "Nodes/DialogueGraphNodeBase.h"
#include "Nodes/DialogueGraphNodeEnd.h"
#include "Nodes/DialogueGraphNodeStart.h"
#include "Nodes/DialogueGraphNodeText.h"

DEFINE_LOG_CATEGORY_STATIC(DialogueGraphCompiler, Log, All)

// Update the opened Dialogue Graph Asset with the current graph editor
// Called when opening a Dialogue Graph Asset
void DialogueGraphCompiler::UpdateWorkingAssetFromGraph(UDialogueSystem* WorkingAsset, UEdGraph* WorkingGraphEditor)
{
	if (WorkingAsset == nullptr || WorkingGraphEditor == nullptr) { return; }

	// Create a new runtime graph with the current Dialogue Graph Asset
	UDialogueSystemRuntimeGraph* RuntimeGraph = NewObject<UDialogueSystemRuntimeGraph>(WorkingAsset);
	WorkingAsset->Graph = RuntimeGraph;

	TArray<TPair<FGuid, FGuid>> Connections;
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
				TPair<FGuid, FGuid> Connection = TPair<FGuid, FGuid>(UIPin->PinId, UIPin->LinkedTo[0]->PinId);
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
		const UDialogueGraphNodeBase* UiGraphNode = Cast<UDialogueGraphNodeBase>(UiNode);
		RuntimeNode->NodeInfo = DuplicateObject(UiGraphNode->GetNodeInfo(), RuntimeNode);
		RuntimeNode->NodeType = UiGraphNode->GetNodeType();

		// Add the runtime node to the list of nodes on the runtime graph
		RuntimeGraph->Nodes.Add(RuntimeNode);
	}

	// Loop through all the connections and set the connected pin on the output pin
	for (TPair<FGuid, FGuid> Connection : Connections) {
		UDialogueRuntimeGraphPin* OutputPin = IdToPinMap[Connection.Key];
		UDialogueRuntimeGraphPin* InputPin = IdToPinMap[Connection.Value];

		OutputPin->ConnectedPin = InputPin;
	}

	UE_LOG(DialogueGraphCompiler, Log, TEXT("Working Graph Updated."));
}

// Update the graph editor with the current opened Dialogue Graph Asset
void DialogueGraphCompiler::UpdateGraphEditorFromWorkingAsset(UDialogueSystem* WorkingAsset, UEdGraph* WorkingGraphEditor)
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
	TArray<TPair<FGuid, FGuid>> Connections;
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
		case EDialogueNodeType::GameActionNode:
			NewNode = NewObject<UDialogueGraphNodeGameAction>(WorkingGraphEditor);
			break;
		default:
			// Log an error for unknown node types
			UE_LOG(DialogueGraphCompiler, Error, TEXT("Unknown node type in UpdateGraphEditorFromWorkingAsset."));
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
			UE_LOG(DialogueGraphCompiler, Error,
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
				Connections.Add(TPair<FGuid, FGuid>(InputPin->PinId, InputPin->ConnectedPin->PinId));
			}

			IdToPinMap.Add(InputPin->PinId, UIPin);
		}

		// Process output pins
		for (UDialogueRuntimeGraphPin* OutputPin : RuntimeNode->OutputPins) {
			UEdGraphPin* UIPin = NewNode->CreateDialoguePin(EGPD_Output, OutputPin->PinName);
			UIPin->PinId = OutputPin->PinId;

			// Add connection if the output pin is connected
			if (OutputPin->ConnectedPin != nullptr) {
				Connections.Add(TPair<FGuid, FGuid>(OutputPin->PinId, OutputPin->ConnectedPin->PinId));
			}

			IdToPinMap.Add(OutputPin->PinId, UIPin);
		}

		// Add the new node to the graph editor
		WorkingGraphEditor->AddNode(NewNode, true, true);
	}

	// Establish connections between pins based on the collected data
	for (TPair<FGuid, FGuid> Connection : Connections) {
		UEdGraphPin* FromPin = IdToPinMap[Connection.Key];
		UEdGraphPin* ToPin = IdToPinMap[Connection.Value];

		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}
}