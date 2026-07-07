// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "BranchOption.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.h"
#include "DialogueSubsystem.generated.h"

/**
 * Delegate broadcast when a dialogue sequence starts.
 * @param DialogueAsset The dialogue being played.
 * @param PC The local player controller involved.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, UDialogueSystem*, DialogueAsset, APlayerController*, PC);

/**
 * Delegate broadcast when a new line of text needs to be displayed.
 * @param Text The dialogue line.
 * @param Speaker The name of the speaker.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueLineRequested, const FText&, Text, const FText&, Speaker);

/**
 * Delegate broadcast when branch options need to be displayed to the player.
 * @param BranchOptions The available options to choose from.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBranchOptionsRequested, const TArray<FBranchOption>&, BranchOptions);

/**
 * Delegate broadcast when the current dialogue sequence ends.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);

/**
 * Delegate broadcast when the player selects a dialogue option.
 * @param Index The index of the selected option.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, int, Index);

/**
 * Global subsystem that coordinates dialogue events across the game.
 */
UCLASS()
class YADSP_API UDialogueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Event fired when a new dialogue begins.
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnDialogueStarted OnDialogueStarted;
	
	// Event fired when dialogue UI should display a new line.
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnDialogueLineRequested OnDialogueLineRequested;
	
	// Event fired when the dialogue has concluded.
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnDialogueEnded OnDialogueEnded;
	
	// Event fired to notify the system which dialogue option was chosen.
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnOptionSelected OnOptionSelected;
	
	// Event fired when dialogue UI should present branch choices.
	UPROPERTY(BlueprintAssignable, Category="YADSP")
	FOnBranchOptionsRequested OnBranchOptionsRequested;
	
};
