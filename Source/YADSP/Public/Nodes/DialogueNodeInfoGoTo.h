// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfoGoTo.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoGoTo : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetAllLabelNodes"))
	FName LabelName;
	
	UFUNCTION()
	TArray<FString> GetAllLabelNodes() const;
};
