#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueGraphNodeEnd.Generated.h"

UCLASS()
class UDialogueGraphNodeEnd : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override { /* Must be overriden */ return nullptr; }
	virtual UEdGraphPin* CreateDefaultInputPin() override {return nullptr;}
	virtual void CreateDefaultOutputPin() override { /* Don't do anything by default */ }

	virtual void InitNodeInfo(UObject* Output) override { /* Nothing by default */ }
	virtual void SetNodeInfo(class UDialogueNodeInfoBase* NodeInfo) override {/* None by default */}
	virtual UDialogueNodeInfoBase* GetNodeInfo() override { /* None by default */ return nullptr; }

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::Unknown; }
	virtual void OnPropertiesChanged() override{ /* Nothing by default */ }
};
