// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfoLabel.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoLabel : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName LabelName = TEXT("DefaultLabelName");
};
