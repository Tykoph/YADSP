// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "BranchOption.generated.h"

USTRUCT(BlueprintType)
struct YADSP_API FBranchOption
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FText DialogueText;
	UPROPERTY(BlueprintReadOnly)
	FText Tooltip;
	UPROPERTY(BlueprintReadOnly)
	bool bExpressionIsValid;
};
