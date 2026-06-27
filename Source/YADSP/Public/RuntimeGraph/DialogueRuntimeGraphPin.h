// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueRuntimeGraphNode.h"
#include "UObject/NameTypes.h"
#include "DialogueRuntimeGraphPin.generated.h"

/**
 * Represents a pin in the dialogue runtime graph.
 * Contains information about the pin's name, ID, and connections.
 */
UCLASS()
class YADSP_API UDialogueRuntimeGraphPin : public UObject
{
	GENERATED_BODY()

public:
	// The name of the pin used to identify it within the graph.
	UPROPERTY()
	FName PinName;

	// Unique identifier for the pin, used for connections.
	UPROPERTY()
	FGuid PinId;

	// Pointer to the connected pin, if any.
	UPROPERTY()
	TObjectPtr<UDialogueRuntimeGraphPin> ConnectedPin;

	// Pointer to the parent node of this pin.
	UPROPERTY()
	TObjectPtr<UDialogueRuntimeGraphNode> ParentNode;
};
