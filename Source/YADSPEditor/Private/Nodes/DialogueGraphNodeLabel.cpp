// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeLabel.h"

void UDialogueGraphNodeLabel::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("End Node Actions")));

	UDialogueGraphNodeLabel* Node = const_cast<UDialogueGraphNodeLabel*>(this);
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

UEdGraphPin* UDialogueGraphNodeLabel::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName PinCategory = TEXT("Output");
	FName PinSubCategory = TEXT("GoToPin");

	UEdGraphPin* NewPin = CreatePin(
		EGPD_Output,
		PinCategory,
		Name
	);
	NewPin->PinType.PinSubCategory = PinSubCategory;

	return NewPin;
}

UEdGraphPin* UDialogueGraphNodeLabel::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Output, FName(TEXT("Go To")));
}