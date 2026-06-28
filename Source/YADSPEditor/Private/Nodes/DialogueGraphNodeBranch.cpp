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
			FUIAction(FExecuteAction::CreateLambda(
				[Node]()
				{
					Node->GetGraph()->RemoveNode(Node);
				}
			))
		);
	}
}

UEdGraphPin* UDialogueGraphNodeBranch::CreateDialoguePin(EEdGraphPinDirection InPinDirection, FName InPinName)
{
	const FName Category = TEXT("Input");
	const FName SubCategory = TEXT("BranchPin");

	UEdGraphPin* Pin = CreatePin(
		InPinDirection,
		Category,
		InPinName
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
	const FString DefaultResponse = TEXT("Continue");
	const FBranchCondition DefaultBranchCondition;
	CreateDialoguePin(EGPD_Output, FName(DefaultResponse));
	GetDialogueNodeInfo()->BranchOptions.Add(DefaultBranchCondition);
}

void UDialogueGraphNodeBranch::SyncWithNodeResponse()
{
	UDialogueNodeInfoBranch* NodeInfo = GetDialogueNodeInfo();
	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphNodeBranch::SyncWithNodeResponse -> NodeInfo is nullptr"))
		return;
	}
	
	int NumGraphNodePins = Pins.Num() - 1;
	const int NumInfoPins = NodeInfo->BranchOptions.Num();

	while (NumGraphNodePins > NumInfoPins) {
		RemovePinAt(NumGraphNodePins - 1, EGPD_Output);
		NumGraphNodePins--;
	}

	while (NumInfoPins > NumGraphNodePins) {
		CreateDialoguePin(
			EGPD_Output,
			FName(NodeInfo->BranchOptions[NumGraphNodePins].DialogueResponseKey.ToString())
		);
		NumGraphNodePins++;
	}

	int Index = 1;
	for (const FBranchCondition& BranchCondition : NodeInfo->BranchOptions) {
		GetPinAt(Index)->PinName = FName(BranchCondition.DialogueResponseKey.ToString());
		Index++;
	}
}
