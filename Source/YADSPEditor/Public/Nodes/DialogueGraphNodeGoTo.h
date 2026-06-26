// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "YADSP/Public/Nodes/DialogueNodeInfoGoTo.h"
#include "DialogueGraphNodeGoTo.generated.h"

UCLASS()
class UDialogueGraphNodeGoTo : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex("FF1FDD"); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::GoToNode; }
	
	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfoGoTo>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override {
		NodeInfoPtr = Cast<UDialogueNodeInfoGoTo>(NodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }
	UDialogueNodeInfoGoTo* GetDialogueNodeInfo() const { return NodeInfoPtr; }

protected:
	UPROPERTY()
	UDialogueNodeInfoGoTo* NodeInfoPtr = nullptr;
};
