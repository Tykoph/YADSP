#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueGraphNodeStart.generated.h"

UCLASS()
class UDialogueGraphNodeStart : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("Start"));
	};
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::Red; }
	virtual bool CanUserDeleteNode() const override { return false; }

	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::StartNode; }
};
