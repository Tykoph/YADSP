// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueGraphNodeBranch.h"

#include "YADSP.h"

FText UDialogueGraphNodeBranch::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Branch"));
}

void UDialogueGraphNodeBranch::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Branch Node Actions")));

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeBranch*>(this);
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

UEdGraphPin* UDialogueGraphNodeBranch::CreateDefaultInputPin()
{
	return CreateDialoguePin(
		EGPD_Input, 
		FName(TEXT("Input")), 
		FName(TEXT("Input"))
		);
}

void UDialogueGraphNodeBranch::CreateDefaultOutputPin()
{
	const FString DefaultResponse = TEXT("Continue");
	const FBranchCondition DefaultBranchCondition;
	
	CreateDialoguePin(
		EGPD_Output,
		FName(DefaultResponse), 
		FName("Output")
		);
	CreateDialoguePin(
		EGPD_Output,
		FName(DefaultResponse), 
		FName("Output 2")
	);
	
	NodeInfo->BranchOptions.Add(DefaultBranchCondition);
	NodeInfo->BranchOptions.Add(DefaultBranchCondition);
}

void UDialogueGraphNodeBranch::SyncWithNodeResponse()
{
	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphNodeBranch::SyncWithNodeResponse -> NodeInfo is nullptr"))
		return;
	}
	
	int32 NumOutputPins = 0;
	for (const UEdGraphPin* Pin : Pins) {
		if (Pin->Direction == EGPD_Output) {
			NumOutputPins++;
		}
	}
	
	const int32 NumInfoPins = NodeInfo->BranchOptions.Num();

	while (NumOutputPins > NumInfoPins) {
		RemovePinAt(NumOutputPins - 1, EGPD_Output);
		NumOutputPins--;
	}

	while (NumInfoPins > NumOutputPins) {
		CreateDialoguePin(
			EGPD_Output,
			FName(NodeInfo->BranchOptions[NumOutputPins].DialogueResponseKey.ToString()),
			FName("Output")
			);
		NumOutputPins++;
	}

	int32 OptionIndex = 0;
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output && OptionIndex < NodeInfo->BranchOptions.Num()) {
			Pin->PinName = FName(NodeInfo->BranchOptions[OptionIndex].DialogueResponseKey.ToString());
			OptionIndex++;
		}
	}
}
