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
