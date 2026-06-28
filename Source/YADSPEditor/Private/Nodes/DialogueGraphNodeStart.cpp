// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeStart.h"

UEdGraphPin* UDialogueGraphNodeStart::CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName)
{
	const FName PinCategory = TEXT("Output");
	const FName PinSubCategory = TEXT("StartPin");

	UEdGraphPin* NewPin = CreatePin(
		InPinDirection,
		PinCategory,
		InPinName
	);
	NewPin->PinType.PinSubCategory = PinSubCategory;

	return NewPin;
}
