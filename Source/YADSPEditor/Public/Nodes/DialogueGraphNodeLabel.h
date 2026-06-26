// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "Nodes/DialogueNodeInfoLabel.h"
#include "DialogueGraphNodeLabel.generated.h"

UCLASS()
class UDialogueGraphNodeLabel : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(NodeInfoPtr->LabelName); }
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex("FF1FDD"); }
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::LabelNode; }
	
	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfoLabel>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override {
		NodeInfoPtr = Cast<UDialogueNodeInfoLabel>(NodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }
	UDialogueNodeInfoLabel* GetDialogueNodeInfo() const { return NodeInfoPtr; }

protected:
	UPROPERTY()
	UDialogueNodeInfoLabel* NodeInfoPtr = nullptr;
};
