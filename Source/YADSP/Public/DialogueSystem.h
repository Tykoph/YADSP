// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeGraph/DialogueSystemRuntimeGraph.h"
#include "UObject/Object.h"
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

public:
	void SetPreSaveListener(const TFunction<void()>& OnPreSaveListener) { OnPreSaveListenerPtr = OnPreSaveListener; }
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	
	UPROPERTY(EditAnywhere)
	FString DialogueName = TEXT("Dialogue Name");

	// Runtime graph that manages dialogue node execution and connections
	UPROPERTY()
	TObjectPtr<UDialogueSystemRuntimeGraph> Graph = nullptr;

	// DataTable containing speakers names
	UPROPERTY(EditAnywhere, meta=(RowType="GSheetLocDataLine"))
	TObjectPtr<UDataTable> SpeakerDataTable;

	// DataTable containing dialogue text
	UPROPERTY(EditAnywhere, meta=(RowType="GSheetLocDataLine"))
	TObjectPtr<UDataTable> DialogueDataTable;

private:

	// Function pointer to callback executed before saving
	TFunction<void()> OnPreSaveListenerPtr = nullptr;
};
