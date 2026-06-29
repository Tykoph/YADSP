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
	// UEdGraphNode Interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex("FF1FDD"); }
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	
	// UDialogueGraphNodeBase Interface
	virtual void CreateDefaultOutputPin() override;

	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoLabel>(Output); }
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override 
	{
		NodeInfo = Cast<UDialogueNodeInfoLabel>(InNodeInfo);
	}
	
	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::LabelNode; }
	
protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoLabel> NodeInfo = nullptr;
};
