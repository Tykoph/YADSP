#include "DialogueGraphSchema.h"
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

	ContextMenuBuilder.AddAction(NewEndNodeAction);
	ContextMenuBuilder.AddAction(NewTextNodeAction);
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

	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
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
