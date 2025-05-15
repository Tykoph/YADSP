// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

public:
	UDialogueUIController(const FObjectInitializer& ObjectInitializer);
	virtual ~UDialogueUIController() override {}

	/**
	* Creates and returns a new instance of DialogueUIController widget.
	* @param PlayerController - The player controller that will own the widget
	* @return A new instance of DialogueUIController widget
	*/
	static UDialogueUIController* CreateInstance(APlayerController* PlayerController);
	
	/**
	* Checks if the dialogue text needs to be wrapped and adjusts its justification accordingly.
	* If text width exceeds container width, justification will be set to "Left".
	* Otherwise, justification will be set to "Center".
	* @param InDialogueText - Target text block widget to check
	* @param Text - String content to measure for wrapping
	*/
	static void IsTextWrapping(UTextBlock* InDialogueText, const FString& Text);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SpeakerName = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DialogueText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UHorizontalBox* ResponseBox = nullptr;
};

UCLASS()
class UDialogueUILoader : public UObject
{
	GENERATED_BODY()

public:
	UDialogueUILoader();
	virtual ~UDialogueUILoader() override {}

	UPROPERTY()
	UClass* WidgetTemplate = nullptr;
	static const inline FString WidgetPath = TEXT("/YADSP/WBP_ExampleDialogueUI");
};
