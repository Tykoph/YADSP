// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "DialogueSystemRuntimeGraph.generated.h"

class UDialogueRuntimeGraphNode;

/**
 * Container managing the full execution graph structure of a dialogue sequence.
 */
UCLASS()
class YADSP_API UDialogueSystemRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	// The full set of nodes allocated to this specific execution context.
	UPROPERTY()
	TArray<TObjectPtr<UDialogueRuntimeGraphNode>> Nodes;
};
