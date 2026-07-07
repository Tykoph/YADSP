// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueGraphNodeBase.h"

UEdGraphPin* UDialogueGraphNodeBase::CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName,
	const FName InCategory)
{ 
	UEdGraphPin* Pin = CreatePin(
		InPinDirection,
		InCategory,
		InPinName
	);
	Pin->PinType.PinSubCategory = GetPinSubCategory();

	return Pin; 
}

FName UDialogueGraphNodeBase::GetPinSubCategory() const
{
	switch (GetNodeType()) {
		case EDialogueNodeType::StartNode: return FName(TEXT("StartPin"));
		case EDialogueNodeType::EndNode: return FName(TEXT("EndPin"));
		case EDialogueNodeType::TextNode: return FName(TEXT("TextPin"));
		case EDialogueNodeType::GameActionNode: return FName(TEXT("ActionPin"));
		case EDialogueNodeType::BranchNode: return FName(TEXT("BranchPin"));
		case EDialogueNodeType::GoToNode: return FName(TEXT("GoToPin"));
		case EDialogueNodeType::LabelNode: return FName(TEXT("GoToPin"));
		default: return NAME_None;
	}
}
