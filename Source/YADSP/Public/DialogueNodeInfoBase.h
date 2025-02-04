// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueNodeInfoBase.generated.h"

UCLASS()
class YADSP_API UDialogueNodeInfoBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UDialogueSystem* DialogueSystem = nullptr;

	UFUNCTION()
	void SetDialogueSystem(UDialogueSystem* System) { DialogueSystem = System; }
};
