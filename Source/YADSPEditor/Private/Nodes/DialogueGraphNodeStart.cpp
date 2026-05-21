// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeStart.h"

UEdGraphPin* UDialogueGraphNodeStart::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName PinCategory = TEXT("Output");
	FName PinSubCategory = TEXT("StartPin");

	UEdGraphPin* NewPin = CreatePin(
		EGPD_Output,
		PinCategory,
		Name
	);
	NewPin->PinType.PinSubCategory = PinSubCategory;

	return NewPin;
}
