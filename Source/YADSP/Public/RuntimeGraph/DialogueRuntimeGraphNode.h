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

	// Inbound connection interface accepting execution flow from previous nodes.
	UPROPERTY()
	TObjectPtr<UDialogueRuntimeGraphPin> InputPin;
	
	// Outbound connection interfaces determining flow to subsequent nodes.
	UPROPERTY()
	TArray<TObjectPtr<UDialogueRuntimeGraphPin>> OutputPins;

	// Position of the node in the graph, used for layout.
	UPROPERTY()
	FVector2D NodePosition;

	// The associated payload defining the data parameters for this node.
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoBase> NodeInfo = nullptr;
};
