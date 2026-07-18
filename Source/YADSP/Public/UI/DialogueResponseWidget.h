// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSubsystem.h"
#include "GSheetLocSystemRichTextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DialogueResponseWidget.generated.h"

/**
 * Base widget for displaying a single selectable option to the player during branching dialogue.
 */
UCLASS(Abstract, Blueprintable)
class YADSP_API UDialogueResponseWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	/**
	 * Configures the widget visuals and underlying data for a given branch option.
	 * @param InTextRowHandle The label to display.
	 * @param InIndex The zero-based choice index passed to the player upon selection.
	 */
	UFUNCTION()
	void SetDialogueOption(const FGSheetLocSystemLocalizedText& InTextRowHandle, const int InIndex);

	/**
	 * Handler executed when the widget's underlying button receives a click event.
	 */
	UFUNCTION()
	void OnButtonClicked();

	// Interactive underlying button capturing player input.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UButton> ResponseButton = nullptr;

	// Text block reflecting the option's text string.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UGSheetLocSystemRichTextBlock> ResponseButtonText = nullptr;

	// Determines whether this option is currently valid and selectable by the player.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	bool bIsValid = true;
	
	// Hover tooltip content for this response option.
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	FGSheetLocSystemLocalizedText OptionTooltip;
	
private:
	int ButtonIndex;
	
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;
};
