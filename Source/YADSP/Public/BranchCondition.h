// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "GameAction/GameExpression.h"
#include "BranchCondition.generated.h"

USTRUCT()
struct FBranchCondition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	UGameExpression* Expression;
	
	UPROPERTY(EditAnywhere)
	bool bHideIfExpressionFail = false;
	
	bool bExpressionIsValid = false;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetDialogue"))
	FName DialogueResponse = TEXT("Continue");
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTooltip"))
	FName ConditionTooltip = TEXT("Continue");
};
