﻿// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueAction.h"
#include "DialogueCamera.h"
#include "DialogueSystemRuntimeGraph.h"
#include "DialoguePlayer.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDialogueEndCallback, EDialogueAction, Action, FString, Data);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDialogueActionCallback, EDialogueAction, Action, FString, Data);

UCLASS()
class YADSP_API UDialoguePlayer : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void PlayDialogue(class UDialogueSystem* DialogueAsset, APlayerController* PlayerController, TArray<ADialogueCamera*> Cameras, FDialogueEndCallback OnDialogueEnded);

	FTimerManager& GetWorldTimerManager() const { return GetWorld()->GetTimerManager(); }
	void ChooseOptionAtIndex(int Index);
	static float CalculateSkipTimer(const FText& Text);
	void AutoSkipDialogue(float Time);

private:
	UPROPERTY()
	class UDialogueSystem* DialogueAssetPtr = nullptr;

	UPROPERTY()
	class UDialogueRuntimeGraphNode* CurrentNodePtr = nullptr;

	UPROPERTY()
	class UDialogueUIController* DialogueUIPtr = nullptr;

	UPROPERTY()
	class APlayerController* PlayerControllerPtr = nullptr;

	float CurrentSkipTime = 0.0f;

	FTimerHandle AutoSkipTimerHandle;

	FDialogueEndCallback OnDialogueEndedCallback;
};
