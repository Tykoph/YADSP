// Copyright 2026 Tom Duby. All Rights Reserved.

#include "DialogueGraphSchema.h"

#include "Nodes/DialogueGraphNodeBranch.h"
#include "Nodes/DialogueGraphNodeGameAction.h"
#include "Nodes/DialogueGraphNodeEnd.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Nodes/DialogueGraphNodeStart.h"
#include "Nodes/DialogueGraphNodeBranch.h"

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewEndNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeEnd::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("new End Node")),
			FText::FromString(TEXT("Makes a new End node")),
			0
		)
	);

	TSharedPtr<FNewNodeAction> NewTextNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeText::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Dialogue Node")),
			FText::FromString(TEXT("Makes a new Dialogue node")),
			0
		)
	);

	TSharedPtr<FNewNodeAction> NewActionNodeGameAction(
		new FNewNodeAction(
			UDialogueGraphNodeGameAction::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New GameAction Node")),
			FText::FromString(TEXT("Makes a new GameAction node")),
			0
		)
	);
	
	TSharedPtr<FNewNodeAction> NewActionNodeBranch(
		new FNewNodeAction(
			UDialogueGraphNodeBranch::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Branch Node")),
			FText::FromString(TEXT("Makes a new Branch node")),
			0
		)
	);
	
	ContextMenuBuilder.AddAction(NewEndNodeAction);
	ContextMenuBuilder.AddAction(NewTextNodeAction);
	ContextMenuBuilder.AddAction(NewActionNodeGameAction);
	ContextMenuBuilder.AddAction(NewActionNodeBranch);
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins are null"));
	}

	if (A->Direction == B->Direction) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of the same direction"));
	}

	if (A->GetOwningNode() == B->GetOwningNode()) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of the same node"));
	}

	if (B->Direction == EGPD_Output) {
		if (B->LinkedTo.Num() > 0) {
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT("Break B"));
		}
	}

	if (A->Direction == EGPD_Output) {
		if (A->LinkedTo.Num() > 0) {
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("Break A"));
		}
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UDialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	UDialogueGraphNodeStart* StartNode = NewObject<UDialogueGraphNodeStart>(&Graph);
	StartNode->CreateNewGuid();
	StartNode->NodePosX = 0;
	StartNode->NodePosY = 0;

	StartNode->CreateDialoguePin(EGPD_Output, FName(TEXT("Start")));

	Graph.AddNode(StartNode, true, true);
	Graph.Modify();
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	// Create a new node instance of the specified class
	UDialogueGraphNodeBase* ResultNode = NewObject<UDialogueGraphNodeBase>(ParentGraph, ClassTemplatePtr);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;
	ResultNode->InitNodeInfo(ResultNode);

	// Setup node info and dialogue system reference
	UDialogueNodeInfoBase* NodeInfo = ResultNode->GetNodeInfo();
	NodeInfo->DialogueSystem = Cast<UDialogueSystem>(ParentGraph->GetOuter());

	// Create input and output pins
	UEdGraphPin* InputPin = ResultNode->CreateDefaultInputPin();
	ResultNode->CreateDefaultOutputPin();

	// Connect to an existing pin if provided
	if (FromPin != nullptr) {
		ResultNode->GetSchema()->TryCreateConnection(InputPin, FromPin);
	}

	// Add node to the graph
	ParentGraph->Modify();
	ParentGraph->AddNode(ResultNode, true, bSelectNewNode);

	return ResultNode;
}
