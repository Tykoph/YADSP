// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueGraphNodeBranch.h"

FText UDialogueGraphNodeBranch::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Branch"));
}

void UDialogueGraphNodeBranch::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Branch Node Actions")));

	UDialogueGraphNodeBranch* Node = const_cast<UDialogueGraphNodeBranch*>(this);
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
	);}

UEdGraphPin* UDialogueGraphNodeBranch::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	const FName Category = TEXT("Input");
	const FName SubCategory = TEXT("BranchPin");

	UEdGraphPin* Pin = CreatePin(
		Dir,
		Category,
		Name
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeBranch::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Input")));
}

void UDialogueGraphNodeBranch::CreateDefaultOutputPin()
{
	FString DefaultResponse = TEXT("Continue");
	FBranchCondition DefaultBranchCondition;
	CreateDialoguePin(EGPD_Output, FName(DefaultResponse));
	GetDialogueNodeInfo()->BranchOptions.Add(DefaultBranchCondition);}

void UDialogueGraphNodeBranch::SyncWithNodeResponse()
{
	UDialogueNodeInfoBranch* NodeInfo = GetDialogueNodeInfo();
	int NumGraphNodePins = Pins.Num() - 1;
	int NumInfoPins = NodeInfo->BranchOptions.Num();

	while (NumGraphNodePins > NumInfoPins) {
		RemovePinAt(NumGraphNodePins - 1, EGPD_Output);
		NumGraphNodePins--;
	}

	while (NumInfoPins > NumGraphNodePins) {
		CreateDialoguePin(
			EGPD_Output,
			FName(NodeInfo->BranchOptions[NumGraphNodePins].DialogueResponse.ToString())
		);
		NumGraphNodePins++;
	}

	int Index = 1;
	for (const FBranchCondition& BranchCondition : NodeInfo->BranchOptions) {
		GetPinAt(Index)->PinName = FName(BranchCondition.DialogueResponse.ToString());
		Index++;
	}
}
