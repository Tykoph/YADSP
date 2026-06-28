// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "GameAction/GameExpression.h"
#include "BranchCondition.generated.h"

USTRUCT()
struct FBranchCondition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UGameExpression> Expression;
	
	UPROPERTY(EditAnywhere)
	bool bHideIfExpressionFail = false;
	
	UPROPERTY()
	bool bExpressionIsValid = false;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTextList"))
	FName DialogueResponseKey = TEXT("Continue");
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTextList"))
	FName ConditionTooltipKey = TEXT("Continue");
};
