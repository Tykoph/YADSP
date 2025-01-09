#include "DialogueGraphNodeEnd.h"

FText UDialogueGraphNodeEnd::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfoPtr != nullptr && NodeInfoPtr->Action != EDialogueNodeAction::None)
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

	return FText::FromString(TEXT("End"));
}

void UDialogueGraphNodeEnd::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("End Node Actions")));

	UDialogueGraphNodeEnd* Node = const_cast<UDialogueGraphNodeEnd*>(this);
	Section.AddMenuEntry(
		"DeleteEntry",
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Delete this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeleteNodeIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] (){
				Node->GetGraph()->RemoveNode(Node);
			}
		))
	);
}

UEdGraphPin* UDialogueGraphNodeEnd::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName Category = TEXT("Input");
	FName SubCategory = TEXT("EndPin");

	UEdGraphPin* Pin = CreatePin(
		Dir,
		Category,
		Name
		);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeEnd::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Finish")));
}
