// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

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
	FString DialogueName = TEXT("Dialogue Name");

	// Runtime graph that manages dialogue node execution and connections
	UPROPERTY()
	UDialogueSystemRuntimeGraph* Graph = nullptr;

	// DataTable containing speakers names
	UPROPERTY(EditAnywhere, meta=(RowType="GSheetLocDataLine"))
	UDataTable* SpeakerDataTable;

	// DataTable containing dialogue text
	UPROPERTY(EditAnywhere, meta=(RowType="GSheetLocDataLine"))
	UDataTable* DialogueDataTable;
	
	/**
	 * Sets a callback function to be executed before the dialogue system is saved
	 * @param OnPreSaveListener Function to be called before saving
	 */
	void SetPreSaveListener(const TFunction<void()>& OnPreSaveListener) { OnPreSaveListenerPtr = OnPreSaveListener; }

	/**
	 * Called before the dialogue system is saved
	 * Executes the registered pre-save listener if one exists
	 * @param SaveContext Context information for the save operation
	 */
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

private: // Members

	// Function pointer to callback executed before saving
	TFunction<void()> OnPreSaveListenerPtr = nullptr;
};
