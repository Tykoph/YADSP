// Copyright Tom Duby. All Rights Reserved.
 
#pragma once

#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

/**
 * Custom graph pin for dialogue text connections
 * Displays with blue color
 */
class SDialogueGraphTextPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphTextPin) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the text pin widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InPin Pointer to the corresponding editor graph pin.
	 */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

protected:
	virtual FSlateColor GetPinColor() const override;
};

/**
 * Custom graph pin for dialogue start node connections
 * Displays with red color
 */
class SDialogueGraphStartPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphStartPin) {}
	SLATE_END_ARGS()
	
	/**
	 * Constructs the start pin widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InPin Pointer to the corresponding editor graph pin.
	 */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

protected:
	virtual FSlateColor GetPinColor() const override;
};

/**
 * Custom graph pin for dialogue end node connections
 * Displays with purple color
 */
class SDialogueGraphEndPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphEndPin) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the end pin widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InPin Pointer to the corresponding editor graph pin.
	 */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

protected:
	virtual FSlateColor GetPinColor() const override;
};

/**
 * Custom graph pin for dialogue action node connections
 * Displays with green color
 */
class SDialogueGraphActionPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphActionPin) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the action pin widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InPin Pointer to the corresponding editor graph pin.
	 */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

protected:
	virtual FSlateColor GetPinColor() const override;
};

/**
 * Custom graph pin for Branch node connections
 * Displays with Orange color
 */
class SDialogueGraphBranchPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphBranchPin) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the branch pin widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InPin Pointer to the corresponding editor graph pin.
	 */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

protected:
	virtual FSlateColor GetPinColor() const override;
};

/**
 * Custom graph pin for GoTo and Label node connections
 * Displays with Pink color
 */
class SDialogueGraphGoToPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphGoToPin) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the goto pin widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InPin Pointer to the corresponding editor graph pin.
	 */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

protected:
	virtual FSlateColor GetPinColor() const override;
};