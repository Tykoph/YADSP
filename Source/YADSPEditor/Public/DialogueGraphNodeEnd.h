#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfoEnd.h"
#include "DialogueGraphNodeEnd.Generated.h"

UCLASS()
class UDialogueGraphNodeEnd : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Purple; }
	virtual bool CanUserDeleteNode() const override {return true;}
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

public: // Our Interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;

	virtual void InitNodeInfo(UObject* Output) override {NodeInfoPtr = NewObject<UDialogueNodeInfoEnd>(Output);}
	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override {NodeInfoPtr = Cast<UDialogueNodeInfoEnd>(NodeInfo);}
	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::EndNode; }
	virtual void OnPropertiesChanged() override{ Modify(); }

protected:
	UPROPERTY()
	UDialogueNodeInfoEnd* NodeInfoPtr = nullptr;
};
