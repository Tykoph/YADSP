#pragma once

#include "DialogueSystem.h"
#include "CoreMinimal.h"
#include "DialogueActor.Generated.h"

UCLASS(Blueprintable)
class YADSP_API ADialogueActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UDialogueSystem* DialogueSystem = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<ACameraActor*> CameraActors;
};
