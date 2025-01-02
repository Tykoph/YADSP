#pragma once

#include "CoreMinimal.h"
#include "DialogueRuntimeGraph.h"
#include "UObject/Object.h"
#include "DialogueGraph.generated.h"

UCLASS(Blueprintable, BlueprintType)
class YADSP_API UDialogueGraph : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString GraphName = "New Dialogue Graph";

	UPROPERTY(EditAnywhere)
	TArray<ACameraActor*> Cameras;


public:
	FString GetGraphName() const { return GraphName; }

	UPROPERTY()
	UDialogueRuntimeGraph* Graph = nullptr;
};
