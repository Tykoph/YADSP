#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfo.h"
#include "DialogueGraphNode.generated.h"

UCLASS()
class UDialogueGraphNode : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override {return true;}
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::DialogueNode; }
	virtual void OnPropertiesChanged() override { SyncWithNodeResponse(); }

public: // Our Interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	void SyncWithNodeResponse();

	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfo>(Output); }
	virtual void SetNodeInfo(class UDialogueNodeInfoBase* NodeInfo) override {NodeInfoPtr = Cast<UDialogueNodeInfo>(NodeInfoPtr);}
	virtual UDialogueNodeInfoBase* GetNodeInfo() override { return NodeInfoPtr; }
	virtual UDialogueNodeInfo* GetDialogueNodeInfo() { return NodeInfoPtr; }

protected:
	UPROPERTY()
	class UDialogueNodeInfo* NodeInfoPtr = nullptr;
};
