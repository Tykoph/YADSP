// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "GameActionBase.h"
#include "DialogueNodeInfoGameAction.generated.h"

/**
 * Defines whether multiple game actions execute in parallel or sequentially.
 */
UENUM()
enum class EGameActionExecutionMode : uint8
{
	Sequence,
	Parallel
};

/**
 * Node info class responsible for storing game actions executed during dialogue.
 */
UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoGameAction : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// Determines how multiple actions will execute relative to each other.
	UPROPERTY(EditAnywhere, Category="YADSP")
	EGameActionExecutionMode GameActionExecutionMode = EGameActionExecutionMode::Sequence;
	
	// Array of game actions triggered by this dialogue node.
	UPROPERTY(EditAnywhere, Instanced, Category="YADSP")
	TArray<TObjectPtr<UGameActionBase>> GameActions;
};
