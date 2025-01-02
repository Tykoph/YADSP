#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNode.generated.h"

UCLASS()
class UDialogueGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override {return FText::FromString(TEXT("NodeTitle")); }
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override {return true;}
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name);
};
