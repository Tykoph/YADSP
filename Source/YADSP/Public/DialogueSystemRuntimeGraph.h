﻿#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "UObject//NameTypes.h"
#include "DialogueSystemRuntimeGraph.generated.h"


UCLASS()
class YADSP_API UDialogueRuntimeGraphPin : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FGuid PinGuid;

	UPROPERTY()
	UDialogueRuntimeGraphPin* Connection = nullptr;
};

UCLASS()
class YADSP_API UDialogueRuntimeGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	EDialogueNodeType NodeType = EDialogueNodeType::DialogueNode;

	UPROPERTY()
	UDialogueRuntimeGraphPin* InputPin;

	UPROPERTY()
	TArray<UDialogueRuntimeGraphPin*> OutputPins;

	UPROPERTY()
	FVector2D NodePosition;

	UPROPERTY()
	UDialogueNodeInfoBase* NodeInfo;
};

UCLASS()
class YADSP_API UDialogueSystemRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UDialogueRuntimeGraphNode*> Nodes;
};