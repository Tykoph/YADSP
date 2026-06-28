// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeText.h"
#include "YADSP/Public/Nodes/DialogueNodeInfoText.h"

#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"
#include "YADSP.h"

FText UDialogueGraphNodeText::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphNodeText::GetNodeTitle -> NodeInfo is nullptr"))
		return FText::FromString(TEXT("null"));
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

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeText*>(this);
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

UEdGraphPin* UDialogueGraphNodeText::CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName)
{
	const FName Category = TEXT("Input");
	const FName SubCategory = TEXT("TextPin");

	UEdGraphPin* Pin = CreatePin(
		InPinDirection,
		Category,
		InPinName
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
	const FString DefaultResponse = TEXT("Continue");
	CreateDialoguePin(EGPD_Output, FName(DefaultResponse));
}
