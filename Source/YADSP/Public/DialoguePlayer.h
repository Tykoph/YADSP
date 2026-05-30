// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueAction.h"
#include "DialogueSubsystem.h"
#include "DialogueSystemRuntimeGraph.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "DialoguePlayer.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDialogueEndCallback, EDialogueAction, Action, FString, Data);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDialogueActionCallback, EDialogueAction, Action, FString, Data);

/**
 * @brief A component that manages dialogue playback within the game.
 */
UCLASS()
class YADSP_API UDialoguePlayer : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Starts playing the given dialogue asset.
	 *
	 * Will start playing the dialogue asset with the given player controller.
	 * When the dialogue ends, the given callback will be called with the action and data of the action.
	 *
	 * @param DialogueAsset The dialogue asset to play.
	 * @param PlayerController The player controller to use for the dialogue.
	 * @param OnDialogueEnded The callback to call when the dialogue ends.
	 */
	UFUNCTION()
	void PlayDialogue(UDialogueSystem* DialogueAsset, APlayerController* PlayerController, const FDialogueEndCallback& OnDialogueEnded);

	/**
	 * @brief Chooses the option at the given index in the current dialogue node.
	 *
	 * Will choose the option at the given index in the current dialogue node.
	 * If the option is a connection to another node, the player will be moved to that node.
	 * If the option is an action, the action will be called with the given data.
	 *
	 * @param Index The index of the option to choose.
	 */
	UFUNCTION()
	void ChooseOptionAtIndex(int Index);

	/**
	 * Calculates a time in seconds based on the length of the given FText.
	 *
	 * @param Text The FText to calculate the skip timer for
	 * @return A float representing the time in seconds until the dialogue should automatically be skipped.
	 */
	static float CalculateSkipTimer(const FString& Text);

	/**
	 * Sets up a timer to automatically skip the current dialogue after a given amount of time.
	 *
	 * @param Time The amount of time to wait before automatically skipping the dialogue.
	 */
	void AutoSkipDialogue(float Time);

private:
	UPROPERTY()
	UDialogueSystem* DialogueAssetPtr = nullptr;

	UPROPERTY()
	UDialogueRuntimeGraphNode* CurrentNodePtr = nullptr;

	UPROPERTY()
	APlayerController* PlayerControllerPtr = nullptr;

	UPROPERTY()
	UDialogueSubsystem* DialogueSubsystem;
	
	float CurrentSkipTime = 0.0f;

	FTimerHandle AutoSkipTimerHandle;

	FDialogueEndCallback OnDialogueEndedCallback;
	
	
	void AutoSkipDialogueSelector(const UDialogueNodeInfoText* NodeInfo);
};
