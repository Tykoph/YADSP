// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeGameAction.h"

FText UDialogueGraphNodeGameAction::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfo != nullptr && !NodeInfo->GameActions.IsEmpty())
	{
		FString Result;
		if (NodeInfo->GameActions.Num() == 1) {
			if (!NodeInfo->GameActions[0])
				return FText::FromString(TEXT("GameAction"));
			FString ActionDataName = NodeInfo->GameActions[0]->GetActionDisplayName().ToString();
			if (ActionDataName.Len() > 15) {
				ActionDataName = ActionDataName.Left(15) + TEXT("...");
			}
			Result = ActionDataName;
		}
		else {
			for (const auto& GameAction : NodeInfo->GameActions) {
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
			switch (NodeInfo->GameActionExecutionMode) {
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
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("GameAction Node Actions")));

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeGameAction*>(this);
	if (auto* Node = WeakNode.Get()) {
		Section.AddMenuEntry(
		   "DeleteEntry",
		   FText::FromString(TEXT("Delete Node")),
		   FText::FromString(TEXT("Delete this node")),
		   FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeleteNodeIcon")),
		   FUIAction(
				FExecuteAction::CreateLambda([Node]() { Node->GetGraph()->RemoveNode(Node); }),
				FCanExecuteAction::CreateLambda([Node]() { return Node->CanUserDeleteNode(); })
			)
	   );
	}
}

UEdGraphPin* UDialogueGraphNodeGameAction::CreateDefaultInputPin()
{
	return CreateDialoguePin(
		EGPD_Input,
		FName(TEXT("Input")),
		FName(TEXT("Input"))
		);
}

void UDialogueGraphNodeGameAction::CreateDefaultOutputPin()
{
	CreateDialoguePin(
		EGPD_Output, 
		FName(TEXT("Output")), 
		FName(TEXT("Output"))
		);
}
