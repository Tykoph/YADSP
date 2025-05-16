// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "DialogueGraphSchema.generated.h"

/**
 * Graph schema defining rules for creating and connecting nodes in the dialogue graph editor.
 * Handles graph context actions, node connections validation, and default node creation.
 */
UCLASS()
class UDialogueGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	// Retrieves available actions for the graph context menu.
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	// Validates if two pins can be connected and returns the appropriate response.
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	// Creates default nodes when a new graph is created
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
};

/**
 * Handles the creation of specific node types based on a class template.
 * Provides functionality to perform the node creation action at a specified location in the graph.
 */
USTRUCT()
struct FNewNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public :
	/**
	 * Constructor for creating a new node action.
	 * @param ClassTemplate The class template to use for creating the new node
	 * @param InNodeCategory Category of the node in the context menu
	 * @param InMenuDesc Description shown in the context menu
	 * @param InToolTip Tooltip text shown when hovering over the menu item
	 * @param InGrouping Grouping priority for menu item ordering
	 */
	FNewNodeAction(UClass* ClassTemplate, FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping) :
		FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), ClassTemplatePtr(ClassTemplate) { }

	/**
	 * Creates and adds a new node to the graph based on the class template.
	 * @param ParentGraph The graph where the new node will be created
	 * @param FromPin The source pin if creating a connection, or nullptr
	 * @param Location The position in the graph where the node should be placed
	 * @param bSelectNewNode Whether the newly created node should be selected
	 * @return The newly created graph node instance
	 */
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

protected:
	UClass* ClassTemplatePtr = nullptr;
};
