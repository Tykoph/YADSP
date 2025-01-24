// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueGraphSchema.h"
#include "DialogueGraphNodeAction.h"
#include "DialogueGraphNodeAnimation.h"
#include "DialogueGraphNodeCamera.h"
#include "DialogueGraphNodeStart.h"
#include "DialogueGraphNodeEnd.h"
#include "DialogueGraphNodeText.h"

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

	TSharedPtr<FNewNodeAction> NewActionNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeAction::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Action Node")),
			FText::FromString(TEXT("Makes a new Action node")),
			0
		)
	);

	TSharedPtr<FNewNodeAction> NewCameraNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeCamera::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Camera Node")),
			FText::FromString(TEXT("Makes a new Camera node")),
			0
		)
	);

	TSharedPtr<FNewNodeAction> NewAnimationNodeAction(
		new FNewNodeAction(
			UDialogueGraphNodeAnimation::StaticClass(),
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("New Animation Node")),
			FText::FromString(TEXT("Makes a new Animation node")),
			0
		)
	);

	ContextMenuBuilder.AddAction(NewEndNodeAction);
	ContextMenuBuilder.AddAction(NewTextNodeAction);
	ContextMenuBuilder.AddAction(NewActionNodeAction);
	ContextMenuBuilder.AddAction(NewCameraNodeAction);
	ContextMenuBuilder.AddAction(NewAnimationNodeAction);
}

const FPinConnectionResponse UDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins are null"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of the same direction"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of the same node"));
	}

	if (B->Direction == EGPD_Output)
	{
		if (B->LinkedTo.Num() > 0)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT("Break B"));
		}
	}

	if (A->Direction == EGPD_Output)
	{
		if (A->LinkedTo.Num() > 0)
		{
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
	UDialogueGraphNodeBase* ResultNode = NewObject<UDialogueGraphNodeBase>(ParentGraph, ClassTemplatePtr);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;
	ResultNode->InitNodeInfo(ResultNode);

	UEdGraphPin* InputPin = ResultNode->CreateDefaultInputPin();
	ResultNode->CreateDefaultOutputPin();

	if (FromPin != nullptr)
	{
		ResultNode->GetSchema()->TryCreateConnection(InputPin, FromPin);
	}

	ParentGraph->Modify();
	ParentGraph->AddNode(ResultNode, true, bSelectNewNode);

	return ResultNode;
}
