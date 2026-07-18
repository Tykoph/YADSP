// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueResponseWidget.h"
#include "DialogueSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "GSheetLocSystemRichTextBlock.h"
#include "DialogueWidget.generated.h"

/**
 * Controller class for handling dialogue UI widgets in the game.
 * Manages the display of speaker names, dialogue text, and response options.
 */
UCLASS(Abstract, Blueprintable)
class YADSP_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:	
	/**
	 * Updates the UI text elements with the latest conversation state.
	 * @param TextRowHandle The line currently spoken.
	 * @param SpeakersRowHandle The names of the active speaker.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="YADSP")
	void UpdateDisplay(const FGSheetLocSystemLocalizedText& TextRowHandle, const FGSheetLocSystemLocalizedText& SpeakersRowHandle);
	
	/**
	 * Configures and surfaces interactive widgets whenever a choice sequence arises.
	 * @param InBranchOptions Array of validated available branching paths.
	 */
	UFUNCTION(BlueprintCallable, Category="YADSP")
	void OnBranchOptionsRequested(const TArray<FBranchOption>& InBranchOptions);
	
	/**
	 * Closes inputs and begins the UI phase-out for the conclusion of the dialogue.
	 */
	UFUNCTION()
	void OnDialogueEnded();

	/**
	 * Removes or deactivates a specific dialogue option from the layout box.
	 * @param Index The numerical index of the option.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="YADSP")
	void ClearDialogueOption(int Index);
	
	// The text element reflecting the name of the active speaker(s).
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UGSheetLocSystemRichTextBlock> SpeakerTextBlock = nullptr;
	
	// The text element reflecting the current spoken line.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UGSheetLocSystemRichTextBlock> DialogueTextBlock = nullptr;

	// Container actively organizing selectable choice widgets.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UPanelWidget> ResponseBox = nullptr;
	
	// Blueprint template class to dynamically instantiate for each response.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="YADSP")
	TSubclassOf<UDialogueResponseWidget> DialogueOptionWidgetClass;
	
	// Maintained list tracking currently active choice widgets.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	TArray<TObjectPtr<UUserWidget>> DialogueOptionsWidgets;
	
protected:
	// Internal list backing the specific text values of presented branches.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	TArray<FText> DialogueOptions;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;
};
