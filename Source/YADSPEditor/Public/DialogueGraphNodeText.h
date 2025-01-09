#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfoText.h"
#include "DialogueGraphNodeText.Generated.h"

UCLASS()
class UDialogueGraphNodeText : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override {return true;}
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

public: // UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::TextNode; }
	virtual void OnPropertiesChanged() override{ SyncWithNodeResponse(); }

public: // Our Interface
	void SyncWithNodeResponse();

	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfoText>(Output);}
	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override {NodeInfoPtr = Cast<UDialogueNodeInfoText>(NodeInfo);}
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }
	UDialogueNodeInfoText* GetDialogueNodeInfo() const { return NodeInfoPtr; }


protected:
	UPROPERTY()
	UDialogueNodeInfoText* NodeInfoPtr = nullptr;
};
