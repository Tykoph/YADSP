// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueGraphFactories.h"
#include "EdGraph/EdGraphPin.h"
#include "UI/SDialogueGraphPins.h"
#include "UI/SDialogueGraphNodeText.h"
#include "Nodes/DialogueGraphNodeText.h"

FDialoguePinFactory::~FDialoguePinFactory()
{
}

TSharedPtr<SGraphPin> FDialoguePinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if (Pin->PinType.PinSubCategory == FName(TEXT("TextPin"))) {
		return SNew(SDialogueGraphTextPin, Pin);
	}
	if (Pin->PinType.PinSubCategory == FName(TEXT("StartPin"))) {
		return SNew(SDialogueGraphStartPin, Pin);
	}
	if (Pin->PinType.PinSubCategory == FName(TEXT("EndPin"))) {
		return SNew(SDialogueGraphEndPin, Pin);
	}
	if (Pin->PinType.PinSubCategory == FName(TEXT("ActionPin"))) {
		return SNew(SDialogueGraphActionPin, Pin);
	}
	if (Pin->PinType.PinSubCategory == FName(TEXT("BranchPin"))) {
		return SNew(SDialogueGraphBranchPin, Pin);
	}
	if (Pin->PinType.PinSubCategory == FName(TEXT("GoToPin"))) {
		return SNew(SDialogueGraphGoToPin, Pin);
	}

	return nullptr;
}

FDialogueGraphNodeFactory::~FDialogueGraphNodeFactory()
{
}

TSharedPtr<SGraphNode> FDialogueGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(Node)) {
		return SNew(SDialogueGraphNodeText, TextNode);
	}
	return nullptr;
}