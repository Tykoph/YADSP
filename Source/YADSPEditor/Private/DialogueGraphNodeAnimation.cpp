// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueGraphNodeAnimation.h"

FText UDialogueGraphNodeAnimation::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfoPtr != nullptr && NodeInfoPtr->Action != EDialogueAction::None)
	{
		FString Result = UEnum::GetDisplayValueAsText(NodeInfoPtr->Action).ToString();
		if (!NodeInfoPtr->ActionData.IsEmpty())
		{
			FString ActionData = NodeInfoPtr->ActionData;
			if (ActionData.Len() > 15)
			{
				ActionData = ActionData.Left(15) + TEXT("...");
			}
			Result += TEXT(": ") + ActionData;
		}

		return FText::FromString(Result);
	}

	return FText::FromString(TEXT("Animation"));
}

void UDialogueGraphNodeAnimation::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Animation Node Actions")));

	UDialogueGraphNodeAnimation* Node = const_cast<UDialogueGraphNodeAnimation*>(this);
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

UEdGraphPin* UDialogueGraphNodeAnimation::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName Category = TEXT("Input");
	FName SubCategory = TEXT("AnimationPin");

	UEdGraphPin* Pin = CreatePin(
		Dir,
		Category,
		Name
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeAnimation::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Input")));
}

void UDialogueGraphNodeAnimation::CreateDefaultOutputPin()
{
	CreateDialoguePin(EGPD_Output, FName(TEXT("Output")));
}
