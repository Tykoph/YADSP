// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueNodeType.generated.h"

UENUM()
enum class EDialogueNodeType
{
	Unknown,
	StartNode,
	DialogueNode,
	EndNode,
	ActionNode,
	TextNode
};
