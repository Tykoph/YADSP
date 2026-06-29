// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeGoTo.h"

#include "YADSP.h"

FText UDialogueGraphNodeGoTo::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphNodeGoTo::GetNodeTitle -> NodeInfo is nullptr"))
		return FText::FromString(TEXT("null"));
	}
	return FText::FromString((TEXT("GoTo %s"), NodeInfo->LabelName.ToString()));
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
		   FUIAction(
				FExecuteAction::CreateLambda([Node]() { Node->GetGraph()->RemoveNode(Node); }),
				FCanExecuteAction::CreateLambda([Node]() { return Node->CanUserDeleteNode(); })
			)
	   );
	}
}

UEdGraphPin* UDialogueGraphNodeGoTo::CreateDefaultInputPin()
{
	return CreateDialoguePin(		
		EGPD_Input, 
		FName(TEXT("Go To")), 
		FName(TEXT("Input"))
		);
}
