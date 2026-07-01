// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "Nodes/DialogueNodeInfoText.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeText.generated.h"

UCLASS()
class UDialogueGraphNodeText : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode iInterface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase iInterface
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;
	
	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoText>(Output); }
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override
	{
		NodeInfo = Cast<UDialogueNodeInfoText>(InNodeInfo);
	}

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::TextNode; }

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoText> NodeInfo = nullptr;
};
