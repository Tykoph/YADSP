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
		return ConvertTextAToStringA(DialogueSystem->SpeakerStringArray);
	}

	UFUNCTION()
	int GetSpeakerIndex() const
	{
		const TArray<FString> Speakers = GetSpeakerArray();

		if (DialogueSystem == nullptr)
		{
			return -1;
		}
		if (Speaker.IsEmpty())
		{
			return -1;
		}
		return Speakers.Find(Speaker);
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

	UFUNCTION()
	FText GetSpeakerName(const FString& SpeakerName) const
	{
		FText NullText;

		if (DialogueSystem == nullptr)
		{
			return NullText;
		}
		if (SpeakerName.IsEmpty())
		{
			return NullText;
		}

		const int SpeakerIndex = DialogueSystem->CameraStringArray.Find(SpeakerName);
		return DialogueSystem->SpeakerStringArray[SpeakerIndex];
	}

private:
	// A bit janky, but hey, it supposed to work
	// TODO: double-check behavior with localized text
	static TArray<FString> ConvertTextAToStringA(const TArray<FText>& TextArray)
	{
		TArray<FString> NewArray;

		for (FText Text : TextArray)
		{
			NewArray.Add(Text.ToString());
		}

		return NewArray;
	}
};
