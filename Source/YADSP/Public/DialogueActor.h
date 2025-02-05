// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "CoreMinimal.h"
#include "DialogueActor.Generated.h"

UCLASS(Blueprintable)
class YADSP_API ADialogueActor : public AActor
{
	GENERATED_BODY()

	ADialogueActor();
	virtual ~ADialogueActor() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UPROPERTY(EditAnywhere)
	UDialogueSystem* DialogueSystem = nullptr;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<ACameraActor*> CameraActors;

private:
	UFUNCTION()
	void UpdateArrayLenght();
};
