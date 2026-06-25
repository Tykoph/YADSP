// Copyright 2026 Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoBranch.h"

TArray<FString> UDialogueNodeInfoBranch::GetDialogue() const
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

TArray<FString> UDialogueNodeInfoBranch::GetTooltip() const
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
