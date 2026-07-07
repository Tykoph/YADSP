// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "Nodes/DialogueNodeInfoBase.h"
#include "DialogueGraphNodeBase.generated.h"

/**
 * Base class for all graph nodes in the dialogue editor.
 */
UCLASS()
class UDialogueGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:
	/**
	 * Creates a specific dialogue pin.
	 * @param InPinDirection Direction of the pin.
	 * @param InPinName Name of the pin.
	 * @param InCategory Category of the pin.
	 * @return Pointer to the created pin.
	 */
	virtual UEdGraphPin* CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName, const FName InCategory);

	/**
	 * Gets the subcategory FName for this node's pins.
	 * @return The pin subcategory name.
	 */
	virtual FName GetPinSubCategory() const;

	/** Creates the default input pin for this node if applicable. */
	virtual UEdGraphPin* CreateDefaultInputPin() { return nullptr; }
	
	/** Creates the default output pin for this node if applicable. */
	virtual void CreateDefaultOutputPin() { /* Don't do anything by default */ }
	
	/** Checks whether this node should return runtime info. */
	virtual bool ShouldReturnInfo() const { return true; }
	
	/**
	 * Initializes the runtime node info based on this graph node's data.
	 * @param Output The output object to initialize.
	 */
	virtual void InitNodeInfo(UObject* Output) { /* Nothing by default */ }
	
	/** Gets the associated runtime node information object. */
	virtual UDialogueNodeInfoBase* GetNodeInfo() const { return nullptr; }
	
	/**
	 * Sets the runtime node information object for this node.
	 * @param NodeInfo The info to assign.
	 */
	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) { /* None by default */ }
	
	/** Whether the user can delete this node from the graph. */
	virtual bool CanUserDeleteNode() const override { return true; };

	/**
	 * Gets the specific node type for this graph node.
	 * @return The node type enum value.
	 */
	virtual EDialogueNodeType GetNodeType() const { return EDialogueNodeType::Unknown; }
	
	/** Callback triggered when the node's properties have been modified. */
	virtual void OnPropertiesChanged() { /* Nothing by default */ }
	
	/** Reference to the owning dialogue system asset. */
	TWeakObjectPtr<UDialogueSystem> DialogueSystem = nullptr;
};
