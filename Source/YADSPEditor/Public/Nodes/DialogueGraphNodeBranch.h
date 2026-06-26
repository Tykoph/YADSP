// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once
#include "DialogueGraphNodeBase.h"
#include "Nodes/DialogueNodeInfoBranch.h"
#include "DialogueGraphNodeBranch.generated.h"

UCLASS()
class UDialogueGraphNodeBranch : public UDialogueGraphNodeBase
{
	GENERATED_BODY()
	
public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Orange; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::BranchNode; }
	virtual void OnPropertiesChanged() override { SyncWithNodeResponse(); }

	// YADSP Interface
	void SyncWithNodeResponse();
	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfoBranch>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override
	{
		NodeInfoPtr = Cast<UDialogueNodeInfoBranch>(NodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }
	UDialogueNodeInfoBranch* GetDialogueNodeInfo() const { return NodeInfoPtr; }

protected:
	UPROPERTY()
	UDialogueNodeInfoBranch* NodeInfoPtr = nullptr;

};
