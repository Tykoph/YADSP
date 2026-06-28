// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeLabel.h"

#include "YADSP.h"

FText UDialogueGraphNodeLabel::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphNodeLabel::GetNodeTitle -> NodeInfo is nullptr"))
		return FText::FromString(TEXT("Nullptr"));
	}
	return FText::FromString(NodeInfo->LabelName.ToString());
}

void UDialogueGraphNodeLabel::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Label Node Actions")));

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeLabel*>(this);
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

UEdGraphPin* UDialogueGraphNodeLabel::CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName)
{
	const FName PinCategory = TEXT("Output");
	const FName PinSubCategory = TEXT("GoToPin");

	UEdGraphPin* NewPin = CreatePin(
		InPinDirection,
		PinCategory,
		InPinName
	);
	NewPin->PinType.PinSubCategory = PinSubCategory;

	return NewPin;
}

UEdGraphPin* UDialogueGraphNodeLabel::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Output, FName(TEXT("Go To")));
}