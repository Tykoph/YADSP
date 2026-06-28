// Copyright Tom Duby. All Rights Reserved.

#include "DialogueGraphSchema.h"

#include "YADSP.h"
#include "Nodes/DialogueGraphNodeBranch.h"
#include "Nodes/DialogueGraphNodeGameAction.h"
#include "Nodes/DialogueGraphNodeEnd.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Nodes/DialogueGraphNodeStart.h"
#include "Nodes/DialogueGraphNodeGoTo.h"
#include "Nodes/DialogueGraphNodeLabel.h"

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewEndNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeEnd::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New End Node")),
			FText::FromString(TEXT("Makes a new End Node")),
			0
		)
	);

	TSharedPtr<FNewNodeAction> NewTextNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeText::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Dialogue Node")),
			FText::FromString(TEXT("Makes a new Dialogue Node")),
			0
		)
	);

	TSharedPtr<FNewNodeAction> NewGameActionNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeGameAction::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New GameAction Node")),
			FText::FromString(TEXT("Makes a new GameAction Node")),
			0
		)
	);
	
	TSharedPtr<FNewNodeAction> NewBranchNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeBranch::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Branch Node")),
			FText::FromString(TEXT("Makes a new Branch Node")),
			0
		)
	);
	
	TSharedPtr<FNewNodeAction> NewGoToNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeGoTo::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New GoTo Node")),
			FText::FromString(TEXT("Makes a new GoTo Node")),
			0
		)
	);
	
	TSharedPtr<FNewNodeAction> NewLabelNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeLabel::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Label Node")),
			FText::FromString(TEXT("Makes a new Label Node")),
			0
		)
	);
	
	ContextMenuBuilder.AddAction(NewTextNodeAction);
	ContextMenuBuilder.AddAction(NewGameActionNodeAction);
	ContextMenuBuilder.AddAction(NewBranchNodeAction);
	ContextMenuBuilder.AddAction(NewGoToNodeAction);
	ContextMenuBuilder.AddAction(NewLabelNodeAction);
	ContextMenuBuilder.AddAction(NewEndNodeAction);
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	if (PinA == nullptr || PinB == nullptr) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins are null"));
	}

	if (PinA->Direction == PinB->Direction) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of the same direction"));
	}

	if (PinA->GetOwningNode() == PinB->GetOwningNode()) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of the same node"));
	}

	if (PinB->Direction == EGPD_Output) {
		if (PinB->LinkedTo.Num() > 0) {
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT("Break B"));
		}
	}

	if (PinA->Direction == EGPD_Output) {
		if (PinA->LinkedTo.Num() > 0) {
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("Break A"));
		}
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UDialogueGraphSchema::CreateDefaultNodesForGraph(UEdGraph& InGraph) const
{
	UDialogueGraphNodeStart* StartNode = NewObject<UDialogueGraphNodeStart>(&InGraph);
	StartNode->CreateNewGuid();
	StartNode->NodePosX = 0;
	StartNode->NodePosY = 0;

	StartNode->CreateDialoguePin(EGPD_Output, FName(TEXT("Start")));

	InGraph.AddNode(StartNode, true, true);
	InGraph.Modify();
}

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D InLocation, bool bSelectNewNode)
{
	// Create a new node instance of the specified class
	UDialogueGraphNodeBase* ResultNode = NewObject<UDialogueGraphNodeBase>(ParentGraph, ClassTemplatePtr);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = InLocation.X;
	ResultNode->NodePosY = InLocation.Y;
	ResultNode->InitNodeInfo(ResultNode);

	// Setup node info and dialogue system reference
	UDialogueNodeInfoBase* NodeInfo = ResultNode->GetNodeInfo();
	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("FNewNodeAction::PerformAction -> NodeInfo is nullptr"));
		return nullptr;
	}
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
