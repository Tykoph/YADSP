// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfoLabel.generated.h"

/**
 * Node info class defining a jump target destination within the graph.
 */
UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoLabel : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	// The unique identifier for this label target.
	UPROPERTY(EditAnywhere, Category="YADSP")
	FName LabelName = TEXT("DefaultLabelName");
};
