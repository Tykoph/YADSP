// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "GameExpression.h"
#include "BranchCondition.generated.h"

USTRUCT()
struct FBranchCondition
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Instanced, Category="YADSP")
	TObjectPtr<UGameExpression> Expression;
	
	UPROPERTY(EditAnywhere, Category="YADSP")
	bool bHideIfExpressionFail = false;
	
	UPROPERTY()
	bool bExpressionIsValid = false;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTextList"), Category="YADSP")
	FName DialogueResponseKey = TEXT("Continue");
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetTooltipList"), Category="YADSP")
	FName ConditionTooltipKey = TEXT("None");
};
