// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "BranchOption.generated.h"

/**
 * Represents an available dialogue branch option presented to the player.
 */
USTRUCT(BlueprintType)
struct YADSP_API FBranchOption
{
	GENERATED_BODY()
	
public:
	// The text displayed for this dialogue option.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	FText DialogueText;
	
	// Optional tooltip text providing more context or condition requirements.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	FText Tooltip;
	
	// Indicates whether the associated condition expression evaluated successfully.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	bool bExpressionIsValid = false;
};
