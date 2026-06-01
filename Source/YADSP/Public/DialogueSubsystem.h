// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "DialogueSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, UDialogueSystem*, DialogueAsset, APlayerController*, PC);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDialogueLineRequested, FString, Text, FString, Speaker, const TArray<FText>&, Options);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, int, Index);

UCLASS()
class YADSP_API UDialogueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnDialogueStarted OnDialogueStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueLineRequested OnDialogueLineRequested;
	
	UPROPERTY(BlueprintAssignable)
	FOnDialogueEnded OnDialogueEnded;
	
	UPROPERTY(BlueprintAssignable)
	FOnOptionSelected OnOptionSelected;
	
};
