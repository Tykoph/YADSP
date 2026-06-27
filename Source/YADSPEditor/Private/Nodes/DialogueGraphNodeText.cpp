// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeText.h"
#include "YADSP/Public/Nodes/DialogueNodeInfoText.h"

#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"

FText UDialogueGraphNodeText::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(NodeInfoPtr);

	if (!NodeInfo) {
		return FText::FromString(TEXT("NodeInfo null ?????"));
	}
	
	if (NodeInfo->Title.IsEmpty()) {
		FString DialogueTextStr = NodeInfo->DialogueKey.ToString();
		if (DialogueTextStr.Len() > 15) {
			DialogueTextStr = DialogueTextStr.Left(15) + TEXT("...");
		}
		return FText::FromString(DialogueTextStr);
	}

	return FText::FromString(NodeInfo->Title);
}

void UDialogueGraphNodeText::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Text Node Actions")));

	UDialogueGraphNodeText* Node = const_cast<UDialogueGraphNodeText*>(this);
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

UEdGraphPin* UDialogueGraphNodeText::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName Category = TEXT("Input");
	FName SubCategory = TEXT("TextPin");

	UEdGraphPin* Pin = CreatePin(
		Dir,
		Category,
		Name
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeText::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Input")));
}

void UDialogueGraphNodeText::CreateDefaultOutputPin()
{
	FString DefaultResponse = TEXT("Continue");
	CreateDialoguePin(EGPD_Output, FName(DefaultResponse));
}
