#include "DialogueGraphSchema.h"
#include "DialogueGraphNode.h"
#include "DialogueSystemNodeInfo.h"

void UDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewNodeAction(
		new FNewNodeAction(
			FText::FromString(TEXT("Node")),
			FText::FromString(TEXT("new Node")),
			FText::FromString(TEXT("Makes a new node")),
			0
		)
	);

	ContextMenuBuilder.AddAction(NewNodeAction);
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

UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UDialogueGraphNode* ResultNode = NewObject<UDialogueGraphNode>(ParentGraph);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;

	ResultNode->SetNodeInfo(NewObject<UDialogueSystemNodeInfo>(ResultNode));

	UEdGraphPin* InputPin = ResultNode->CreateDialoguePin(EGPD_Input, TEXT("Display"));
	FString DefaultResponse = TEXT("Continue");
	ResultNode->GetNodeInfo()->DialogueResponses.Add(FText::FromString(DefaultResponse));
	ResultNode->SyncWithNodeResponse();

	if (FromPin != nullptr)
	{
		ResultNode->GetSchema()->TryCreateConnection(InputPin, FromPin);
	}

	ParentGraph->Modify();
	ParentGraph->AddNode(ResultNode, true, bSelectNewNode);

	return ResultNode;
}
