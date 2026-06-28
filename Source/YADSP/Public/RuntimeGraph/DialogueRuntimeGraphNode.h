// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "DialogueNodeType.h"
#include "Nodes/DialogueNodeInfoBase.h"
#include "DialogueRuntimeGraphNode.generated.h"

class UDialogueRuntimeGraphPin;

/**
 * Represents a node in the dialogue runtime graph.
 * Contains properties for node type, input and output pins, position, and associated node information.
 */
UCLASS()
class YADSP_API UDialogueRuntimeGraphNode : public UObject
{
	GENERATED_BODY()

public:
	// Type of the dialogue node, indicating its purpose in the graph.
	UPROPERTY()
	EDialogueNodeType NodeType = EDialogueNodeType::Unknown;

	// Pointer to the input pin of the node, if any.
	UPROPERTY()
	TObjectPtr<UDialogueRuntimeGraphPin> InputPin;
	
	// Array of output pins connected to this node.
	UPROPERTY()
	TArray<TObjectPtr<UDialogueRuntimeGraphPin>> OutputPins;

	// Position of the node in the graph, used for layout.
	UPROPERTY()
	FVector2D NodePosition;

	// Pointer to additional information about the node, if any.
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoBase> NodeInfo = nullptr;
};
