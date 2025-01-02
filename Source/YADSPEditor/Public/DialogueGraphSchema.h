﻿#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "DialogueGraphSchema.generated.h"

UCLASS()
class UDialogueGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
};

USTRUCT()
struct FNewNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FNewNodeAction() {}
	FNewNodeAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
};