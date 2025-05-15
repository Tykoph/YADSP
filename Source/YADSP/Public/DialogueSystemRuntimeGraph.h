// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "DialogueNodeType.h"
#include "DialogueSystemRuntimeGraph.generated.h"


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
	UDialogueRuntimeGraphPin* ConnectedPin;

	// Pointer to the parent node of this pin.
	UPROPERTY()
	class UDialogueRuntimeGraphNode* ParentNode;
};

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
	EDialogueNodeType NodeType = EDialogueNodeType::DialogueNode;

	// Pointer to the input pin of the node, if any.
	UPROPERTY()
	UDialogueRuntimeGraphPin* InputPin;

	// Array of output pins connected to this node.
	UPROPERTY()
	TArray<UDialogueRuntimeGraphPin*> OutputPins;

	// Position of the node in the graph, used for layout.
	UPROPERTY()
	FVector2D NodePosition;

	// Pointer to additional information about the node, if any.
	UPROPERTY()
	class UDialogueNodeInfoBase* NodeInfo = nullptr;
};

UCLASS()
class YADSP_API UDialogueSystemRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	// Array of nodes in the graph.
	UPROPERTY()
	TArray<UDialogueRuntimeGraphNode*> Nodes;
};
