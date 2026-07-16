// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "Nodes/DialogueNodeInfoGameAction.h"
#include "DialogueGraphNodeGameAction.generated.h"

/**
 * Represents a game action node in the dialogue graph, triggering specific events or logic.
 */
UCLASS()
class UDialogueGraphNodeGameAction : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: 
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Green; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual void InitNodeInfo(UObject* Output) override
	{
		NodeInfo = NewObject<UDialogueNodeInfoGameAction>(Output, NAME_None, RF_Transactional);
	}
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override
	{
		NodeInfo = Cast<UDialogueNodeInfoGameAction>(InNodeInfo);
	}

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::GameActionNode; }
	
	virtual void OnPropertiesChanged() override { Modify(); }
	// /////////

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoGameAction> NodeInfo = nullptr;
};
