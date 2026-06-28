// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeEnd.h"

FText UDialogueGraphNodeEnd::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End"));
}

void UDialogueGraphNodeEnd::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("End Node Actions")));

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeEnd*>(this);
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

UEdGraphPin* UDialogueGraphNodeEnd::CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName)
{
	const FName Category = TEXT("Input");
	const FName SubCategory = TEXT("EndPin");

	UEdGraphPin* Pin = CreatePin(
		InPinDirection,
		Category,
		InPinName
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeEnd::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Finish")));
}
