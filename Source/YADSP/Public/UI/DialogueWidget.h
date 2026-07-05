// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueResponseWidget.h"
#include "DialogueSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlock.h"
#include "DialogueWidget.generated.h"

/**
 * Controller class for handling dialogue UI widgets in the game.
 * Manages the display of speaker names, dialogue text, and response options.
 * Provides functionality for text wrapping and justification based on content size.
 */
UCLASS(Abstract, Blueprintable)
class YADSP_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="YADSP")
	void UpdateDisplay(const FText& InText, const FText& InSpeaker);
	
	UFUNCTION(BlueprintCallable, Category="YADSP")
	void OnBranchOptionsRequested(const TArray<FBranchOption>& InBranchOptions);
	
	/**
	* Checks if the dialogue text needs to be wrapped and adjusts its justification accordingly.
	* If text width exceeds container width, justification will be set to "Left".
	* Otherwise, justification will be set to "Center".
	* @param InDialogueTextBlock - Target text block widget to check
	* @param InText - String content to measure for wrapping
	*/
	UFUNCTION(BlueprintCallable, Category="YADSP")
	static void UpdateTextWrapping(URichTextBlock* InDialogueTextBlock, const FString& InText);
	
	UFUNCTION()
	void OnDialogueEnded();

	UFUNCTION(BlueprintNativeEvent, Category="YADSP")
	void ClearDialogueOption(int Index);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<URichTextBlock> SpeakerTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<URichTextBlock> DialogueTextBlock = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UPanelWidget> ResponseBox = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="YADSP")
	TSubclassOf<UDialogueResponseWidget> DialogueOptionWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	TArray<TObjectPtr<UUserWidget>> DialogueOptionsWidgets;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	TArray<FText> DialogueOptions;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;
};
