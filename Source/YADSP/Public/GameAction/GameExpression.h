// Copyright Tom Duby. All Rights Reserved.

#pragma once
#include "GameActionBase.h"
#include "GameExpression.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, CollapseCategories)
class YADSP_API UGameExpression : public UGameActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Execute_Implementation(const FGameActionContext& Context) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "GameAction")
	bool ExecuteWithReturn();
};
