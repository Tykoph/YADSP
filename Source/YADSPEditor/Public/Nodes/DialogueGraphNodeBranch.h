// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueGraphNodeBase.h"
#include "Nodes/DialogueNodeInfoBranch.h"
#include "DialogueGraphNodeBranch.generated.h"

UCLASS()
class UDialogueGraphNodeBranch : public UDialogueGraphNodeBase
{
	GENERATED_BODY()
	
public: 
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Orange; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoBranch>(Output); }
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override
	{
		NodeInfo = Cast<UDialogueNodeInfoBranch>(InNodeInfo);
	}

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::BranchNode; }

	virtual void OnPropertiesChanged() override { SyncWithNodeResponse(); }
	
private:
	void SyncWithNodeResponse();

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoBranch> NodeInfo = nullptr;

};
