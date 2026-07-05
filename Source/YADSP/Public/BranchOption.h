// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "BranchOption.generated.h"

USTRUCT(BlueprintType)
struct YADSP_API FBranchOption
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	FText DialogueText;
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	FText Tooltip;
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	bool bExpressionIsValid = false;
};
