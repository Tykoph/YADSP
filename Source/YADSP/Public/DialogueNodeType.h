﻿#pragma once

#include "DialogueNodeType.generated.h"

UENUM()
enum class EDialogueNodeType
{
	Unknown,
	StartNode,
	DialogueNode,
	EndNode,
	ActionNode
};
