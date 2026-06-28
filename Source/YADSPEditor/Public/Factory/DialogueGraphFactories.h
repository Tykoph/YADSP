// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

/**
 * Factory for creating visual representations of graph pins in the Dialogue Editor.
 * This class determines which SWidget should be used for different pin types.
 */
struct FDialoguePinFactory : public FGraphPanelPinFactory
{
	virtual ~FDialoguePinFactory() override;
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override;
};

/**
 * Factory for creating visual representations of graph nodes in the Dialogue Editor.
 * This class determines which SWidget should be used for different node types.
 */
struct FDialogueGraphNodeFactory : public FGraphPanelNodeFactory
{
	virtual ~FDialogueGraphNodeFactory() override;
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* Node) const override;
};
