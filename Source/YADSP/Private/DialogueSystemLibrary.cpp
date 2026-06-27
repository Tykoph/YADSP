// Copyright Tom Duby. All Rights Reserved.

#include "DialogueSystemLibrary.h"

#include "DialogueSystem.h"
#include "GSheetLocSystemLibrary.h"

FString UDialogueSystemLibrary::GetTranslatedText(const UDialogueSystem* DialogueSystem, const UDataTable* DataTable, const FName& TextKey)
{
	if (DialogueSystem == nullptr || DataTable == nullptr || TextKey.IsNone()) {
		return TEXT("Data Error") ;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DataTable;
	DataTableRowHandle.RowName = TextKey;
	
	FString LocalizedText = UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle);
	
	return LocalizedText;
}

TArray<FString> UDialogueSystemLibrary::GetTranslatedTextArray(const UDialogueSystem* DialogueSystem, const UDataTable* DataTable, const TArray<FName>& TextKeys)
{
	if (DialogueSystem == nullptr || DataTable == nullptr || TextKeys.IsEmpty()) {
		return TArray<FString>();
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DataTable;
	TArray<FString> LocalizedText;
	
	for (const FName TextKey : TextKeys) {
		if (!TextKey.IsNone()) {
			DataTableRowHandle.RowName = TextKey;
			LocalizedText.Add(UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle));
		}
		else {
			LocalizedText.Add(TEXT("TextKey is None"));
		}
	} 
		
	return LocalizedText;
}
