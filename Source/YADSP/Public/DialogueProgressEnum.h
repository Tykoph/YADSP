// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueProgressEnum.generated.h"

// TODO : Make it a global option for a project and possibility to be override by each dialogue.
/**
 * Defines the rules for automatically progressing dialogue lines.
 */
UENUM()
enum class EDialogueProgression : uint8
{
	WaitForInput,
	AutoAfterText,
	AutoAfterSound,
	AutoAfterTime
};

