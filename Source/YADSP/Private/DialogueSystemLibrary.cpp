// Copyright Tom Duby. All Rights Reserved.

#include "DialogueSystemLibrary.h"
#include "DialogueSystem.h"
#include "GSheetLocSystemLibrary.h"
#include "YADSP.h"

FString UDialogueSystemLibrary::GetTranslatedText(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const FName InTextKey)
{
	if (InDialogueSystem == nullptr || InDataTable == nullptr || InTextKey.IsNone()) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueSystemLibrary::GetTranslatedText -> %s%s%s"), 
			InDialogueSystem ? TEXT("") : TEXT("DialogueSystem is Null. "),
			(InDataTable ? TEXT("") : TEXT("DataTable is Null. ")),
			(InTextKey.IsNone() ? TEXT("TextKey is Empty. ") : TEXT(""))
			);
		return TEXT("Data Error") ;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = InDataTable;
	DataTableRowHandle.RowName = InTextKey;
	
	FString LocalizedText = UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle);
	
	return LocalizedText;
}

TArray<FString> UDialogueSystemLibrary::GetTranslatedTextArray(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const TArray<FName>& InTextKeys)
{
	if (InDialogueSystem == nullptr || InDataTable == nullptr || InTextKeys.IsEmpty()) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueSystemLibrary::GetTranslatedText -> %s%s%s"), 
			InDialogueSystem ? TEXT("") : TEXT("DialogueSystem is Null. "),
			(InDataTable ? TEXT("") : TEXT("DataTable is Null. ")),
			(InTextKeys.IsEmpty() ? TEXT("TextKey is Empty. ") : TEXT(""))
			);
		return TArray<FString>();
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = InDataTable;
	
	TArray<FString> LocalizedText;
	LocalizedText.Reserve(InTextKeys.Num());
	
	for (const FName TextKey : InTextKeys) {
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
