// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "YADSP/Public/Nodes/DialogueNodeInfoText.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeText.generated.h"

UCLASS()
class UDialogueGraphNodeText : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection InPinDirection, FName InPinName) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::TextNode; }
	virtual void OnPropertiesChanged() override { }
	
	virtual void InitNodeInfo(UObject* Output) override { NodeInfo = NewObject<UDialogueNodeInfoText>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* InNodeInfo) override
	{
		NodeInfo = Cast<UDialogueNodeInfoText>(InNodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfo; }
	UDialogueNodeInfoText* GetDialogueNodeInfo() const { return NodeInfo; }

protected:
	UPROPERTY()
	TObjectPtr<UDialogueNodeInfoText> NodeInfo = nullptr;
};
