// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueGraphNodeBase.generated.h"

UCLASS()
class UDialogueGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:
	// Must be overridden
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) { return nullptr; }

	virtual UEdGraphPin* CreateDefaultInputPin() { return nullptr; }
	virtual void CreateDefaultOutputPin() { /* Don't do anything by default */ }
	virtual void InitNodeInfo(UObject* Output) { /* Nothing by default */ }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) { /* None by default */ }

	virtual UDialogueNodeInfoBase* GetNodeInfo() const { return nullptr; }
	virtual EDialogueNodeType GetNodeType() const { return EDialogueNodeType::Unknown; }

	virtual void OnPropertiesChanged() { /* Nothing by default */ }
};
