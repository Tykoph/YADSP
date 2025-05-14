// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

// TODO : Make it a global option for a project and possiblity to be overriden by each dialogue.
UENUM()
enum class ESkipDialogue : uint8
{
	NoSkip,
	AutoSkipBasedOnText,
	AutoSkipAfterSound,
	AutoSkipAfterTime
};

