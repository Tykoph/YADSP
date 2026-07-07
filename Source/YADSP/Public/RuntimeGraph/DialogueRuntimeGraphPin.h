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
	// Internal display name, often used to differentiate distinct exit paths.
	UPROPERTY()
	FName PinName;

	// Unique identifier for the pin, used for connections.
	UPROPERTY()
	FGuid PinId;

	// The counterpart pin linked across the graph. Null if unlinked.
	UPROPERTY()
	TObjectPtr<UDialogueRuntimeGraphPin> ConnectedPin;

	// The node that owns and manages this pin object.
	UPROPERTY()
	TObjectPtr<UDialogueRuntimeGraphNode> ParentNode;
};
