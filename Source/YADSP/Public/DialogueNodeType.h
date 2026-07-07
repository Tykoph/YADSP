// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueNodeType.generated.h"

/**
 * Defines the types of nodes available in a dialogue graph.
 */
UENUM()
enum class EDialogueNodeType : uint8
{
	Unknown,
	StartNode,
	EndNode,
	TextNode,
	BranchNode,
	GameActionNode,
	GoToNode,
	LabelNode
};
