#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "DialogueSystemRuntimeGraph.h"
#include "UObject/Object.h"
#include "DialogueSystem.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueSystem : public UObject
{
	GENERATED_BODY()

public: // Properties
	UPROPERTY(EditAnywhere)
	FString DialogueName = TEXT("Enter Dialogue Name here");

	UPROPERTY()
	UDialogueSystemRuntimeGraph* Graph = nullptr;

	UPROPERTY()
	ACameraActor* DefaultCamera = nullptr;

public: // Our Interface
	void SetPreSaveListener(std::function<void()> OnPreSaveListener){OnPreSaveListenerPtr = OnPreSaveListener; }

public : // UObject interface
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

private: // Members
	std::function<void()> OnPreSaveListenerPtr = nullptr;
};
