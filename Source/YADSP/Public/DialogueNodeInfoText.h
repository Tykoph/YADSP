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

	UPROPERTY(EditAnywhere)
	FText Speaker;

	UPROPERTY(EditAnywhere)
	FText DialogueText;

	UPROPERTY(EditAnywhere, meta = (ClampMin = -1))
	int32 CameraIndex = -1;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetCameraStringArray"))
	FString CameraName;

	UFUNCTION()
	TArray<FString> GetCameraStringArray() const
	{
		if (DialogueSystem == nullptr)
		{
			return TArray<FString>();
		}
		return DialogueSystem->CameraStringArray;
	}

	UPROPERTY(EditAnywhere)
	USoundCue* DialogueSound;

	UPROPERTY(EditAnywhere)
	ESkipDialogue SkipDialogue = ESkipDialogue::NoSkip;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SkipDialogue == ESkipDialogue::AutoSkipAfterTime"))
	float SkipAfterSeconds;

	UPROPERTY(EditAnywhere)
	TArray<FText> DialogueResponses;
};
