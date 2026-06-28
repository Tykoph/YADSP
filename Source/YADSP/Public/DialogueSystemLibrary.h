// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "DialogueSystemLibrary.generated.h"

UCLASS()
class YADSP_API UDialogueSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static FString GetTranslatedText(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const FName InTextKey);
	
	UFUNCTION(BlueprintPure)
	static TArray<FString> GetTranslatedTextArray(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const TArray<FName>& InTextKeys);
};
