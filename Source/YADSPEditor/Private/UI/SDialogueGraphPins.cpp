// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/UI/SDialogueGraphPins.h"


FSlateColor SDialogueGraphTextPin::GetPinColor() const
{
	return FColor::Blue;
}

FSlateColor SDialogueGraphStartPin::GetPinColor() const
{
	return FColor::Red;
}

FSlateColor SDialogueGraphEndPin::GetPinColor() const
{
	return FColor::Purple;
}

FSlateColor SDialogueGraphActionPin::GetPinColor() const
{
	return FColor::Green;
}

FSlateColor SDialogueGraphAnimationPin::GetPinColor() const	
{
	return FColor::Yellow;
}
