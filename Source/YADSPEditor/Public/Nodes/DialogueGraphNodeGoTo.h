// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "Nodes/DialogueNodeInfoGoTo.h"
#include "DialogueGraphNodeGoTo.generated.h"

/**
 * Represents a go-to node in the dialogue graph, directing the flow to a specific label.
 */
UCLASS()
class UDialogueGraphNodeGoTo : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: 
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex("FF1FDD"); }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	// /////////

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	
	virtual void InitNodeInfo(UObject* Output) override
	{
		NodeInfo = NewObject<UDialogueNodeInfoGoTo>(Output, NAME_None, RF_Transactional);
	}
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override 
	{
		NodeInfo = Cast<UDialogueNodeInfoGoTo>(InNodeInfo);
	}

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::GoToNode; }

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoGoTo> NodeInfo = nullptr;
};
