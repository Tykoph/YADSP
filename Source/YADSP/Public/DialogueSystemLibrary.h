// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogueSystemLibrary.generated.h"

UCLASS()
class YADSP_API UDialogueSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="YADSP")
	static FString GetTranslatedText(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const FName InTextKey);
	
	UFUNCTION(BlueprintPure, Category="YADSP")
	static TArray<FString> GetTranslatedTextArray(const UDialogueSystem* InDialogueSystem, const UDataTable* InDataTable, const TArray<FName>& InTextKeys);
};
