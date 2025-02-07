// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject//NameTypes.h"
#include "DialogueNodeType.h"
#include "DialogueSystemRuntimeGraph.generated.h"


UCLASS()
class YADSP_API UDialogueRuntimeGraphPin : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FGuid PinId;

	UPROPERTY()
	UDialogueRuntimeGraphPin* Connection = nullptr;

	UPROPERTY()
	class UDialogueRuntimeGraphNode* Parent = nullptr;
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
	class UDialogueNodeInfoBase* NodeInfo = nullptr;
};

UCLASS()
class YADSP_API UDialogueSystemRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UDialogueRuntimeGraphNode*> Nodes;
};
