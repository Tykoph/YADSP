// Copyright Tom Duby. All Rights Reserved.

#include "GameAction/GameExpression.h"

void UGameExpression::Execute_Implementation(const FGameActionContext& Context)
{
	ExecuteWithReturn();
	Super::Execute_Implementation(Context);
}

bool UGameExpression::ExecuteWithReturn_Implementation()
{
	return true;
}
