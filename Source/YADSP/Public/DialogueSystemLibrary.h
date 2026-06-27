// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "DialogueSystemLibrary.generated.h"

UCLASS()
class YADSP_API UDialogueSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static FString GetTranslatedText(const UDialogueSystem* DialogueSystem, const UDataTable* DataTable, const FName& TextKey);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetTranslatedTextArray(const UDialogueSystem* DialogueSystem, const UDataTable* DataTable, const TArray<FName>& TextKeys);
};
