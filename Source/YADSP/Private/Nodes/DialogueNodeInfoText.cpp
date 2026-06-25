// Copyright 2026 Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoText.h"

#include "DialogueSystem.h"
#include "GSheetLocSystemLibrary.h"

void UDialogueNodeInfoText::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	OnPropertiesChanged.Broadcast();
}

TArray<FString> UDialogueNodeInfoText::GetSpeakerFromTable() const
{
	TArray<FString> Options;

	if (DialogueSystem && DialogueSystem->SpeakerDataTable) {
		TArray<FName> RowNames = DialogueSystem->SpeakerDataTable->GetRowNames();

		for (const FName& Name : RowNames) {
			Options.Add(Name.ToString());
		}
	}

	if (Options.Num() == 0) {
		Options.Add(TEXT("No Table Found"));
	}

	return Options;
}

TArray<FString> UDialogueNodeInfoText::GetDialogueFromTable() const
{
	TArray<FString> Options;

	if (DialogueSystem && DialogueSystem->DialogueDataTable) {
		TArray<FName> RowNames = DialogueSystem->DialogueDataTable->GetRowNames();

		for (const FName& Name : RowNames) {
			Options.Add(Name.ToString());
		}
	}

	if (Options.Num() == 0) {
		Options.Add(TEXT("No Table Found"));
	}

	return Options;
}
