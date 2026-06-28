// Copyright Tom Duby. All Rights Reserved.

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
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection InPinDirection, FName InPinName) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::GoToNode; }
	
	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoGoTo>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override {
		NodeInfo = Cast<UDialogueNodeInfoGoTo>(InNodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	UDialogueNodeInfoGoTo* GetDialogueNodeInfo() const { return NodeInfo; }

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoGoTo> NodeInfo = nullptr;
};
