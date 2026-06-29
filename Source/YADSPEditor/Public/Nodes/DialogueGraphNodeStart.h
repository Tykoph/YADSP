// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueGraphNodeStart.generated.h"

UCLASS()
class UDialogueGraphNodeStart : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	// UEdGraphNode Interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Start")); }
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Red; }
	virtual bool CanUserDeleteNode() const override { return false; }

	// UDialogueGraphNodeBase Interface
	virtual void CreateDefaultOutputPin() override;

	virtual bool ShouldReturnInfo() const override { return false; }
	
	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::StartNode; }
};
