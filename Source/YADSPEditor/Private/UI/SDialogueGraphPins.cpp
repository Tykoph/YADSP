// Copyright Tom Duby. All Rights Reserved.

#include "UI/SDialogueGraphPins.h"


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

FSlateColor SDialogueGraphBranchPin::GetPinColor() const	
{
	return FColor::Orange;
}

FSlateColor SDialogueGraphGoToPin::GetPinColor() const	
{
	return FColor::FromHex("FF1FDD");
}
