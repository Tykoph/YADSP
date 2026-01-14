// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueGraphNodeText.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"
#include "DialogueNodeInfoText.h"

FText UDialogueGraphNodeText::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(NodeInfoPtr);

	if (NodeInfo->Title.IsEmpty()) {
		FString DialogueTextStr = NodeInfo->DialogueID.ToString();
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
	GetDialogueNodeInfo()->DialogueResponses.Add(FText::FromString(DefaultResponse));
}

void UDialogueGraphNodeText::SyncWithNodeResponse()
{
	UDialogueNodeInfoText* NodeInfo = GetDialogueNodeInfo();
	int NumGraphNodePins = Pins.Num() - 1;
	int NumInfoPins = NodeInfo->DialogueResponses.Num();

	while (NumGraphNodePins > NumInfoPins) {
		RemovePinAt(NumGraphNodePins - 1, EGPD_Output);
		NumGraphNodePins--;
	}

	while (NumInfoPins > NumGraphNodePins) {
		CreateDialoguePin(
			EGPD_Output,
			FName(NodeInfo->DialogueResponses[NumGraphNodePins].ToString())
		);
		NumGraphNodePins++;
	}

	int Index = 1;
	for (const FText& Response : NodeInfo->DialogueResponses) {
		GetPinAt(Index)->PinName = FName(Response.ToString());
		Index++;
	}
}
