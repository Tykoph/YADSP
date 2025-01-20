﻿#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfoAction.h"
#include "DialogueGraphNodeAction.Generated.h"

UCLASS()
class UDialogueGraphNodeAction : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Green; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::ActionNode; }
	virtual void OnPropertiesChanged() override { Modify(); }

	// Our Interface
	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfoAction>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override
	{
		NodeInfoPtr = Cast<UDialogueNodeInfoAction>(NodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }

protected:
	UPROPERTY()
	UDialogueNodeInfoAction* NodeInfoPtr = nullptr;
};
