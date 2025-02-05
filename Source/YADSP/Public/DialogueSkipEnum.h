// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

UENUM()
enum class ESkipDialogue : uint8
{
	NoSkip,
	AutoSkipBasedOnText,
	AutoSkipAfterSound,
	AutoSkipAfterTime
};

