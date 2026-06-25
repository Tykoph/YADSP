// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueOption.h"
#include "DialogueSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "DialogueUIController.generated.h"

/**
 * Controller class for handling dialogue UI widgets in the game.
 * Manages the display of speaker names, dialogue text, and response options.
 * Provides functionality for text wrapping and justification based on content size.
 */
UCLASS()
class YADSP_API UDialogueUIController : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateDisplay(const FText& Text, const FText& Speaker);
	
	UFUNCTION(BlueprintCallable)
	void OnBranchOptionsRequested(const TArray<FBranchOption>& BranchOptions);
	
	/**
	* Checks if the dialogue text needs to be wrapped and adjusts its justification accordingly.
	* If text width exceeds container width, justification will be set to "Left".
	* Otherwise, justification will be set to "Center".
	* @param DialogueTextBlock - Target text block widget to check
	* @param Text - String content to measure for wrapping
	*/
	UFUNCTION(BlueprintCallable)
	static void IsTextWrapping(URichTextBlock* DialogueTextBlock, const FString& Text);
	
	UFUNCTION()
	void OnDialogueEnded();

	UFUNCTION()
	void ClearDialogueOption(int Index);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* SpeakerName = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	URichTextBlock* DialogueText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UHorizontalBox* ResponseBox = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDialogueOption> DialogueOptionClass;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<UUserWidget*> DialogueOptionsWidgets;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FText> DialogueOptions;
	
private:
	UPROPERTY()
	UDialogueSubsystem* DialogueSubsystem;
};
