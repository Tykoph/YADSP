// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueGraphFactories.h"

#include "YADSP.h"
#include "UI/SDialogueGraphPins.h"
#include "UI/SDialogueGraphNodeText.h"
#include "UI/SDialogueGraphNodeBranch.h"
#include "EdGraph/EdGraphPin.h"

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

TSharedPtr<SGraphNode> FDialogueGraphNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if (const UDialogueGraphNodeBase* Node = Cast<UDialogueGraphNodeBase>(InNode)) {
		switch (Node->GetNodeType()) {
			case EDialogueNodeType::TextNode:
				return SNew(SDialogueGraphNodeText, Cast<UDialogueGraphNodeText>(InNode));
			case EDialogueNodeType::BranchNode:
				return SNew(SDialogueGraphNodeBranch, Cast<UDialogueGraphNodeBranch>(InNode));
			case EDialogueNodeType::GameActionNode:
				return nullptr;
			default:
				return nullptr;
		}
	}
	
	return nullptr;
}