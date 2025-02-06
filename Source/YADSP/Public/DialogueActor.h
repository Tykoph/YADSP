// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "CoreMinimal.h"
#include "DialoguePlayer.h"
#include "DialogueActor.Generated.h"

class UDialoguePlayer;

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
	TArray<ADialogueCamera*> CameraActors;

	UFUNCTION(BlueprintCallable)
	void PlayDialogue(APlayerController* PlayerController, FDialogueEndCallback OnDialogueEnded) const;

private:
	UFUNCTION()
	void UpdateArrayLenght();

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDialoguePlayer> DialoguePlayer;
};
