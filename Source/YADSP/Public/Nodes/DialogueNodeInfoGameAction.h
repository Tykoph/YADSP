// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "GameActionBase.h"
#include "DialogueNodeInfoGameAction.generated.h"

UENUM()
enum class EGameActionExecutionMode : uint8
{
	Sequence,
	Parallel
};

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoGameAction : public UDialogueNodeInfoBase
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	EGameActionExecutionMode GameActionExecutionMode = EGameActionExecutionMode::Sequence;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UGameActionBase>> GameActions;
};

