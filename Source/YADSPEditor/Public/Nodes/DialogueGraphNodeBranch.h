// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueGraphNodeBase.h"
#include "Nodes/DialogueNodeInfoBranch.h"
#include "DialogueGraphNodeBranch.generated.h"

/**
 * Represents a branching node in the dialogue graph editor, allowing multiple conversational paths.
 */
UCLASS()
class UDialogueGraphNodeBranch : public UDialogueGraphNodeBase
{
	GENERATED_BODY()
	
public: 
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Orange; }
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
	// /////////
	
private:
	/** Synchronizes the node's visual representation with its underlying response data. */
	void SyncWithNodeResponse();

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoBranch> NodeInfo = nullptr;

};
