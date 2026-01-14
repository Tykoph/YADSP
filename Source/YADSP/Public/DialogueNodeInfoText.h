// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueSkipEnum.h"
#include "DialogueNodeInfoText.generated.h"

class USoundCue;

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Title;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetSpeakerOptions"))
	FName SpeakerID;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetDialogueOptions"))
	FName DialogueID;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetCameraStringArray"))
	FString CameraName;

	UPROPERTY(EditAnywhere)
	USoundCue* DialogueSound;

	UPROPERTY(EditAnywhere)
	ESkipDialogue SkipDialogue = ESkipDialogue::NoSkip;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SkipDialogue == ESkipDialogue::AutoSkipAfterTime", ClampMin = 0))
	float SkipAfterSeconds;

	UPROPERTY(EditAnywhere)
	TArray<FText> DialogueResponses;

public:
	UFUNCTION()
	TArray<FString> GetSpeakerOptions() const;

	UFUNCTION()
	FString GetSpeakerName(const FName& SpeakerName) const;

	UFUNCTION()
	TArray<FString> GetDialogueOptions() const;
	
	UFUNCTION()
	FString GetDialogueText(const FName& DialogueText) const;
	
	UFUNCTION()
	TArray<FString> GetCameraStringArray() const;

	UFUNCTION()
	int GetCameraIndex() const;
};
