// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeGoTo.h"

FText UDialogueGraphNodeGoTo::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("GoTo"));
}

void UDialogueGraphNodeGoTo::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("GoTo Node Actions")));

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeGoTo*>(this);
	if (auto* Node = WeakNode.Get()) {
		Section.AddMenuEntry(
		   "DeleteEntry",
		   FText::FromString(TEXT("Delete Node")),
		   FText::FromString(TEXT("Delete this node")),
		   FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeleteNodeIcon")),
		   FUIAction(FExecuteAction::CreateLambda(
			   [Node]()
			   {
				   Node->GetGraph()->RemoveNode(Node);
			   }
		   ))
	   );
	}
}

UEdGraphPin* UDialogueGraphNodeGoTo::CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName)
{
	FName Category = TEXT("Input");
	FName SubCategory = TEXT("GoToPin");

	UEdGraphPin* Pin = CreatePin(
		InPinDirection,
		Category,
		InPinName
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeGoTo::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Go To Node")));
}
