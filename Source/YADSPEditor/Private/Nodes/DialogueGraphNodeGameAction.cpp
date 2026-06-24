// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeGameAction.h"

FText UDialogueGraphNodeGameAction::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfoPtr != nullptr && NodeInfoPtr->GameAction.Num() >= 1 && !NodeInfoPtr->GameAction.IsEmpty())
	{
		FString Result;
		if (NodeInfoPtr->GameAction.Num() == 1) {
			if (!NodeInfoPtr->GameAction[0])
				return FText::FromString(TEXT("GameAction"));
			FString ActionDataName = NodeInfoPtr->GameAction[0]->GetActionDisplayName().ToString();
			if (ActionDataName.Len() > 15) {
				ActionDataName = ActionDataName.Left(15) + TEXT("...");
			}
			Result = ActionDataName;
		}
		else {
			for (const auto GameAction : NodeInfoPtr->GameAction) {
				if (!GameAction)
					continue;
				FString ActionDataName = GameAction->GetActionDisplayName().ToString();
				Result += ActionDataName + TEXT(", ");
			}
			Result.RemoveAt(Result.Len() - 2, 1);
			Result.TrimEndInline();
			if (Result.Len() > 15) {
				Result = Result.Left(15) + TEXT("...");
			}
			switch (NodeInfoPtr->GameActionExecutionMode) {
				case EGameActionExecutionMode::Sequence:
					Result = TEXT("Sequence: ") + Result;
					break;
				case EGameActionExecutionMode::Parallel:
					Result = TEXT("Parallel: ") + Result;
					break;
			}
		}
		return FText::FromString(Result);
	}

	return FText::FromString(TEXT("GameAction"));
}

void UDialogueGraphNodeGameAction::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Node GameActions")));

	UDialogueGraphNodeGameAction* Node = const_cast<UDialogueGraphNodeGameAction*>(this);
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

UEdGraphPin* UDialogueGraphNodeGameAction::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName Category = TEXT("Input");
	FName SubCategory = TEXT("ActionPin");

	UEdGraphPin* Pin = CreatePin(
		Dir,
		Category,
		Name
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeGameAction::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Input")));
}

void UDialogueGraphNodeGameAction::CreateDefaultOutputPin()
{
	CreateDialoguePin(EGPD_Output, FName(TEXT("Output")));
}
