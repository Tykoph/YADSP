// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueGraphNodeEnd.generated.h"

UCLASS()
class UDialogueGraphNodeEnd : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Purple; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection InPinDirection, FName InPinName) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::EndNode; }
	
	virtual bool ShouldReturnInfo() const override { return false; }
};
