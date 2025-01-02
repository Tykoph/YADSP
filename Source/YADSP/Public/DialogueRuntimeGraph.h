#pragma once

#include "CoreMinimal.h"
#include "UObject//NameTypes.h"
#include "DialogueRuntimeGraph.generated.h"

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
	UDialogueRuntimeGraphPin* InputPin;

	UPROPERTY()
	TArray<UDialogueRuntimeGraphPin*> OutputPins;

	UPROPERTY()
	FVector2D NodePosition;
};

UCLASS()
class YADSP_API UDialogueRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UDialogueRuntimeGraphNode*> Nodes;
};