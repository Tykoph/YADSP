// Copyright 2026 Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoText.h"

#include "DialogueSystem.h"
#include "GSheetLocSystemLibrary.h"

TArray<FString> UDialogueNodeInfoText::GetSpeakerOptions() const
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

FString UDialogueNodeInfoText::GetSpeakerName(const FName& SpeakerName) const
{
	FString NullText;

	if (DialogueSystem == nullptr) {
		return NullText;
	}
	if (SpeakerName.IsNone()) {
		return NullText;
	}
	if (DialogueSystem->SpeakerDataTable == nullptr) {
		return NullText;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DialogueSystem->SpeakerDataTable;
	DataTableRowHandle.RowName = SpeakerName;
	
	FString LocalizedName = UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle);
	
	return LocalizedName;
}

TArray<FString> UDialogueNodeInfoText::GetDialogueOptions() const
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

FString UDialogueNodeInfoText::GetDialogueText(const FName& DialogueText) const
{
	FString NullText;

	if (DialogueSystem == nullptr) {
		return NullText;
	}
	if (DialogueText.IsNone()) {
		return NullText;
	}
	if (DialogueSystem->DialogueDataTable == nullptr) {
		return NullText;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DialogueSystem->DialogueDataTable;
	DataTableRowHandle.RowName = DialogueText;
	
	FString LocalizedText = UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle);
	
	return LocalizedText;
}
