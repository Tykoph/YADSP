// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueSkipEnum.h"
#include "DialogueNodeInfoText.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetSpeakerArray"))
	FString Speaker;

	UPROPERTY(EditAnywhere)
	FText DialogueText;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetCameraStringArray"))
	FString CameraName;

	UFUNCTION()
	TArray<FString> GetSpeakerArray() const
	{
		if (DialogueSystem == nullptr)
		{
			return TArray<FString>();
		}
		return DialogueSystem->SpeakerStringArray.ToString();
	}

	UFUNCTION()
	int GetSpeakerIndex() const
	{
		FString StringArray = DialogueSystem->SpeakerStringArray.ToString();
		if (DialogueSystem == nullptr)
		{
			return -1;
		}
		if (Speaker.IsEmpty())
		{
			return -1;
		}
		return DialogueSystem->SpeakerStringArray.Find(Speaker);
	}

	UFUNCTION()
	TArray<FString> GetCameraStringArray() const
	{
		if (DialogueSystem == nullptr)
		{
			return TArray<FString>();
		}
		return DialogueSystem->CameraStringArray;
	}

	UFUNCTION()
	int GetCameraIndex() const
	{
		if (DialogueSystem == nullptr)
		{
			return -1;
		}
		if (CameraName.IsEmpty())
		{
			return -1;
		}
		return DialogueSystem->CameraStringArray.Find(CameraName);
	}

	UPROPERTY(EditAnywhere)
	USoundCue* DialogueSound;

	UPROPERTY(EditAnywhere)
	ESkipDialogue SkipDialogue = ESkipDialogue::NoSkip;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SkipDialogue == ESkipDialogue::AutoSkipAfterTime", ClampMin = 0))
	float SkipAfterSeconds;

	UPROPERTY(EditAnywhere)
	TArray<FText> DialogueResponses;
};
