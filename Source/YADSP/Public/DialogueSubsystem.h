// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "BranchOption.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.h"
#include "DialogueSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, UDialogueSystem*, DialogueAsset, APlayerController*, PC);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueLineRequested, const FText&, Text, const FText&, Speaker);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBranchOptionsRequested, const TArray<FBranchOption>&, BranchOptions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, int, Index);

UCLASS()
class YADSP_API UDialogueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnDialogueStarted OnDialogueStarted;
	
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnDialogueLineRequested OnDialogueLineRequested;
	
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnDialogueEnded OnDialogueEnded;
	
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnOptionSelected OnOptionSelected;
	
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnBranchOptionsRequested OnBranchOptionsRequested;
	
};
