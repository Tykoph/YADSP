// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueAction.h"
#include "DialogueNodeInfoEnd.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoEnd : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EDialogueAction Action = EDialogueAction::None;

	UPROPERTY(EditAnywhere)
	FString ActionData = TEXT("");
};
