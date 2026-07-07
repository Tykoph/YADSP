// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogueSystemLibrary.generated.h"

/**
 * Blueprint function library providing helper utilities for the Dialogue System.
 */
UCLASS()
class YADSP_API UDialogueSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Retrieves a translated text string from a specific data table row.
	 * @param InDialogueSystem The dialogue system context.
	 * @param InDataTable The data table containing the localized strings.
	 * @param InTextKey The row key to look up.
	 * @return The localized string for the specified key.
	 */
	UFUNCTION(BlueprintPure, Category="YADSP")
	static FString GetTranslatedText(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const FName InTextKey);
	
	/**
	 * Retrieves multiple translated text strings from a specific data table.
	 * @param InDialogueSystem The dialogue system context.
	 * @param InDataTable The data table containing the localized strings.
	 * @param InTextKeys The array of row keys to look up.
	 * @return An array of localized strings corresponding to the keys.
	 */
	UFUNCTION(BlueprintPure, Category="YADSP")
	static TArray<FString> GetTranslatedTextArray(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const TArray<FName>& InTextKeys);
};
