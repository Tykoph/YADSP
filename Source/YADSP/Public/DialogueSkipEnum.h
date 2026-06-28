// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSkipEnum.generated.h"

// TODO : Make it a global option for a project and possibility to be override by each dialogue.
UENUM()
enum class ESkipDialogue : uint8
{
	NoSkip,
	AutoSkipBasedOnText,
	AutoSkipAfterSound,
	AutoSkipAfterTime
};

