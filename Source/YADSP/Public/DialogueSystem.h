// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "DialogueSystemRuntimeGraph.h"
#include "UObject/Object.h"
#include "DialogueCamera.h"
#include "DialogueSystem.generated.h"

/**
 * TODO :
 *	Dialogue Sys
 * 		-> String array
 *		-> Update each node with Dialogue Sys ref
 *		-> Update relevant nodes with String Array as a drop down list
 *		-> Update on save
 *		-> match string array index from camera/dialogue node with camera index
 */

UCLASS(BlueprintType)
class YADSP_API UDialogueSystem : public UObject
{
	GENERATED_BODY()

public: // Properties
	UPROPERTY(EditAnywhere)
	FString DialogueName = TEXT("Enter Dialogue Name here");

	UPROPERTY(EditAnywhere)
	bool bAutoSkipEnabled = true;

	UPROPERTY()
	UDialogueSystemRuntimeGraph* Graph = nullptr;

	UPROPERTY()
	AActor* DefaultCamera = nullptr;

	UPROPERTY()
	TArray<ADialogueCamera*> CameraActors;

	UPROPERTY(EditAnywhere)
	TArray<FString> CameraStringArray;

	// Our Interface
	void SetPreSaveListener(std::function<void()> OnPreSaveListener) { OnPreSaveListenerPtr = OnPreSaveListener; }

	// UObject interface
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

private: // Members
	std::function<void()> OnPreSaveListenerPtr = nullptr;
};
