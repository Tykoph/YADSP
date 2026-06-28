// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueGraphFactories.h"

#include "YADSP.h"
#include "EdGraph/EdGraphPin.h"
#include "UI/SDialogueGraphPins.h"
#include "UI/SDialogueGraphNodeText.h"
#include "Nodes/DialogueGraphNodeText.h"

FDialoguePinFactory::~FDialoguePinFactory()
{
}

TSharedPtr<SGraphPin> FDialoguePinFactory::CreatePin(UEdGraphPin* InPin) const
{
	if	(InPin == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("FDialoguePinFactory::CreatePin -> InPin is nullptr"))
		return nullptr;
	}

	// Code optimization
	static const FName TextPinName(TEXT("TextPin"));
	static const FName StartPinName(TEXT("StartPin"));
	static const FName EndPinName(TEXT("EndPin"));
	static const FName ActionPinName(TEXT("ActionPin"));
	static const FName BranchPinName(TEXT("BranchPin"));
	static const FName GoToPinName(TEXT("GoToPin"));

	if (InPin->PinType.PinSubCategory == TextPinName) {
		return SNew(SDialogueGraphTextPin, InPin);
	}
	if (InPin->PinType.PinSubCategory == StartPinName) {
		return SNew(SDialogueGraphStartPin, InPin);
	}
	if (InPin->PinType.PinSubCategory == EndPinName) {
		return SNew(SDialogueGraphEndPin, InPin);
	}
	if (InPin->PinType.PinSubCategory == ActionPinName) {
		return SNew(SDialogueGraphActionPin, InPin);
	}
	if (InPin->PinType.PinSubCategory == BranchPinName) {
		return SNew(SDialogueGraphBranchPin, InPin);
	}
	if (InPin->PinType.PinSubCategory == GoToPinName) {
		return SNew(SDialogueGraphGoToPin, InPin);
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