// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "GameExpression.h"
#include "BranchCondition.generated.h"

/**
 * Represents a single condition used to determine if a dialogue branch should be available.
 */
USTRUCT()
struct FBranchCondition
{
	GENERATED_BODY()
	
public:
	// The game expression evaluated to check if this condition passes.
	UPROPERTY(EditAnywhere, Instanced, Category="YADSP")
	TObjectPtr<UGameExpression> Expression;
	
	// If true, the dialogue option will be hidden rather than disabled if the expression fails.
	UPROPERTY(EditAnywhere, Category="YADSP")
	bool bHideIfExpressionFail = false;
	
	UPROPERTY()
	bool bExpressionIsValid = false;
	
	// Localization key used to retrieve the response text for this branch.
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTextList"), Category="YADSP")
	FName DialogueResponseKey = TEXT("Continue");
	
	// Optional localization key used to retrieve the tooltip text describing this condition.
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTooltipList"), Category="YADSP")
	FName ConditionTooltipKey = TEXT("None");
};
