// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "DialogueSystemRuntimeGraph.generated.h"

class UDialogueRuntimeGraphNode;

UCLASS()
class YADSP_API UDialogueSystemRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	// Array of nodes in the graph.
	UPROPERTY()
	TArray<TObjectPtr<UDialogueRuntimeGraphNode>> Nodes;
};
