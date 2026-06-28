// Copyright Tom Duby. All Rights Reserved.

#include "GameAction/GameExpression.h"

#include "YADSP.h"

void UGameExpression::Execute_Implementation()
{
	ExecuteWithReturn();
	Super::Execute_Implementation();
}

bool UGameExpression::ExecuteWithReturn_Implementation()
{
	UE_LOG(LogYADSP, Warning, TEXT("UGameExpression::ExecuteWithReturn not overridden, returning true"));
	return true;
}
