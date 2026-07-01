// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

/**
 * Factory for creating visual representations of graph pins in the Dialogue Editor.
 * This class determines which SWidget should be used for different pin types.
 */
struct FDialoguePinFactory : FGraphPanelPinFactory
{
	virtual ~FDialoguePinFactory() override {};
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InPin) const override;
};

/**
 * Factory for creating graph nodes with custom slate visual in the Dialogue Editor.
 * This class determines which SWidget should be used for different node types.
 */
struct FDialogueGraphNodeFactory : FGraphPanelNodeFactory
{
	virtual ~FDialogueGraphNodeFactory() override {};
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* InNode) const override;
};
