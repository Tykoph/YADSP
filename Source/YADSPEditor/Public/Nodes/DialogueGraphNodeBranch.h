// Copyright Tom Duby. All Rights Reserved.

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
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection InPinDirection, FName InPinName) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::BranchNode; }
	virtual void OnPropertiesChanged() override { SyncWithNodeResponse(); }

	// YADSP Interface
	void SyncWithNodeResponse();
	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoBranch>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override
	{
		NodeInfo = Cast<UDialogueNodeInfoBranch>(InNodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	UDialogueNodeInfoBranch* GetDialogueNodeInfo() const { return NodeInfo; }

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoBranch> NodeInfo = nullptr;

};
