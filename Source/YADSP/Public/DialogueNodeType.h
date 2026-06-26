// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueNodeType.generated.h"

UENUM()
enum class EDialogueNodeType
{
	Unknown,
	DialogueNode,
	StartNode,
	EndNode,
	TextNode,
	BranchNode,
	GameActionNode,
	GoToNode,
	LabelNode
};
