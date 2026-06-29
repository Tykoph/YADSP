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

public: 
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex("FF1FDD"); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	
	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoGoTo>(Output); }
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
