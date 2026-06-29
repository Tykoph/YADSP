// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/Nodes/DialogueGraphNodeStart.h"

void UDialogueGraphNodeStart::CreateDefaultOutputPin()
{
	CreateDialoguePin(
	EGPD_Output, 
	FName(TEXT("Start")), 
	FName(TEXT("Output"))
	);
}
