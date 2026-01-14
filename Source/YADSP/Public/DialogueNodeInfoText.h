// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueSkipEnum.h"
#include "GSheetLocSystemDefinitions.h"
#include "DialogueNodeInfoText.generated.h"


UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Title;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetSpeakerArray"))
	FName Speaker;

	UPROPERTY(EditAnywhere)
	FText DialogueText;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetCameraStringArray"))
	FString CameraName;

	UFUNCTION()
	TArray<FName> GetSpeakerArray() const
	{
		if (DialogueSystem == nullptr) {
			return TArray<FName>();
		}

		return DialogueSystem->SpeakerStringArray->GetRowNames();
	}

	UFUNCTION()
	int GetSpeakerIndex() const
	{
		const TArray<FName> Speakers = GetSpeakerArray();

		if (DialogueSystem == nullptr) {
			return -1;
		}
		if (Speaker.IsNone()) {
			return -1;
		}

		return Speakers.Find(Speaker);
	}

	UFUNCTION()
	TArray<FString> GetCameraStringArray() const
	{
		if (DialogueSystem == nullptr) {
			return TArray<FString>();
		}
		return DialogueSystem->CameraStringArray;
	}

	UFUNCTION()
	int GetCameraIndex() const
	{
		if (DialogueSystem == nullptr) {
			return -1;
		}
		if (CameraName.IsEmpty()) {
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
	FString GetSpeakerName(const FName& SpeakerName) const
	{
		FString NullText;

		if (DialogueSystem == nullptr) {
			return NullText;
		}
		if (SpeakerName.IsNone()) {
			return NullText;
		}
		FGSheetLocDataLine* Line = DialogueSystem->SpeakerStringArray->FindRow<FGSheetLocDataLine>(SpeakerName, "");
		return Line->english_US;
	}
};
