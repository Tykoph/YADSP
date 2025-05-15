// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueOptionController.generated.h"

/**
 * Controller class for handling individual dialogue response options in the UI.
 * Manages a single response button with associated text and click handling functionality.
 * Used as part of the dialogue system to display and handle player dialogue choices.
 * 
 * Each instance represents one clickable dialogue option that the player can select during conversations.
 * Provides functionality to set up click handlers and manage button states.
 */
UCLASS()
class YADSP_API UDialogueOptionController : public UUserWidget
{
	GENERATED_BODY()

public:
	UDialogueOptionController(const FObjectInitializer& ObjectInitializer);
	virtual ~UDialogueOptionController() override {}

	void SetClickHandler(int Index, std::function<void(int)> ClickHandler);

	UFUNCTION()
	void OnButtonClicked();

	/**
	* Creates and returns a new instance of DialogueOptionController widget.
	* @param PlayerController - The player controller that will own the widget
	* @return A new instance of DialogueOptionController widget
	*/
	static UDialogueOptionController* CreateInstance(APlayerController* PlayerController);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ResponseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ResponseButtonText = nullptr;

private: // Fields
	std::function<void(int)> OnClickHandler;
	int ButtonIndex;
};

UCLASS()
class UDialogueOptionLoader : public UObject
{
	GENERATED_BODY()

public:
	UDialogueOptionLoader();
	virtual ~UDialogueOptionLoader() override {}

	UPROPERTY()
	UClass* WidgetTemplate = nullptr;
	static const inline FString WidgetPath = TEXT("/YADSP/WBP_DialogueOption");
};
