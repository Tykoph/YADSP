#pragma once

#include "CoreMinimal.h"
#include "DialogueSystemRuntimeGraph.h"
#include "UObject/Object.h"
#include "DialogueSystem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class YADSP_API UDialogueSystem : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString GraphName = "New Dialogue Graph";

	UPROPERTY(EditAnywhere)
	TArray<ACameraActor*> Cameras;


public:
	FString GetGraphName() const { return GraphName; }

	UPROPERTY()
	UDialogueSystemRuntimeGraph* Graph = nullptr;
};
