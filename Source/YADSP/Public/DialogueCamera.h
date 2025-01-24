// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "CineCameraSettings.h"
#include "DialogueCamera.generated.h"

UCLASS(Blueprintable, BlueprintType)
class YADSP_API ADialogueCamera : public ACineCameraActor
{
	GENERATED_BODY()

public:
	void SetCameraStats(struct FCineCamStats Stats);


};

USTRUCT()
struct FCineCamStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float CurrentFocalLength = 0.0f;

	UPROPERTY(EditAnywhere)
	float CurrentAperture = 0.0f;

	UPROPERTY(EditAnywhere)
	FCameraFocusSettings FocusSettings;
};
