// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "DialogueSystemRuntimeGraph.h"
#include "UObject/Object.h"
#include "DialogueCamera.h"
#include "DialogueSystem.generated.h"

/**
 * @brief Core class for managing dialogue interactions in the game.
 *
 * The DialogueSystem manages the flow of conversations in the game.
 * It maintains references to speakers, cameras, and the dialogue flow graph that defines conversation structure.
 */
UCLASS(BlueprintType)
class YADSP_API UDialogueSystem : public UObject
{
	GENERATED_BODY()

public: // Properties

	// Name identifier for the dialogue sequence
	UPROPERTY(EditAnywhere)
	FString DialogueName = TEXT("Enter Dialogue Name here");

	// Runtime graph that manages dialogue node execution and connections
	UPROPERTY()
	UDialogueSystemRuntimeGraph* Graph = nullptr;

	// Array of actor references that can be speakers in this dialogue
	UPROPERTY()
	TArray<AActor*> SpeakerActors;

	// Array of text identifiers for speakers, maps positionally to SpeakerActors
	UPROPERTY(EditAnywhere, meta=(RowType="GSheetLocDataLine"))
	UDataTable* SpeakerStringArray;

	// Default camera to use when no specific camera is active.
	// By default, keeps the active camera at the start of the dialogue.
	UPROPERTY()
	AActor* DefaultCamera = nullptr;

	//Array of specialized dialogue cameras available for this dialogue
	UPROPERTY()
	TArray<ADialogueCamera*> CameraActors;

	// String identifiers for cameras, maps positionally to CameraActors
	UPROPERTY(EditAnywhere)
	TArray<FString> CameraStringArray;

	/**
	 * Sets a callback function to be executed before the dialogue system is saved
	 * @param OnPreSaveListener Function to be called before saving
	 */
	void SetPreSaveListener(const std::function<void()>& OnPreSaveListener) { OnPreSaveListenerPtr = OnPreSaveListener; }

	/**
	 * Called before the dialogue system is saved
	 * Executes the registered pre-save listener if one exists
	 * @param SaveContext Context information for the save operation
	 */
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

private: // Members

	// Function pointer to callback executed before saving
	std::function<void()> OnPreSaveListenerPtr = nullptr;
};
