// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "Nodes/DialogueNodeInfoBase.h"
#include "DialogueGraphNodeBase.generated.h"

UCLASS()
class UDialogueGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()

public:
	// Must be overridden
	virtual UEdGraphPin* CreateDialoguePin(const EEdGraphPinDirection InPinDirection, const FName InPinName, const FName InCategory) 
	{ 
		UEdGraphPin* Pin = CreatePin(
			InPinDirection,
			InCategory,
			InPinName
		);
		Pin->PinType.PinSubCategory = GetPinSubCategory();

		return Pin; 
	}

	virtual FName GetPinSubCategory() const
	{
		switch (GetNodeType()) {
			case EDialogueNodeType::StartNode: return FName(TEXT("StartPin"));
			case EDialogueNodeType::EndNode: return FName(TEXT("EndPin"));
			case EDialogueNodeType::TextNode: return FName(TEXT("TextPin"));
			case EDialogueNodeType::GameActionNode: return FName(TEXT("ActionPin"));
			case EDialogueNodeType::BranchNode: return FName(TEXT("BranchPin"));
			case EDialogueNodeType::GoToNode: return FName(TEXT("GoToPin"));
			case EDialogueNodeType::LabelNode: return FName(TEXT("GoToPin"));
			default: return NAME_None;
		}
	}

	virtual UEdGraphPin* CreateDefaultInputPin() { return nullptr; }
	virtual void CreateDefaultOutputPin() { /* Don't do anything by default */ }

	virtual bool ShouldReturnInfo() const { return true; }
	virtual void InitNodeInfo(UObject* Output) { /* Nothing by default */ }
	virtual UDialogueNodeInfoBase* GetNodeInfo() const { return nullptr; }
	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) { /* None by default */ }
	
	virtual EDialogueNodeType GetNodeType() const { return EDialogueNodeType::Unknown; }
	
	virtual void OnPropertiesChanged() { /* Nothing by default */ }

	TWeakObjectPtr<UDialogueSystem> DialogueSystem = nullptr;
};
