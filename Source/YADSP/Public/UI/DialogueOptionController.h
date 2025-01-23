// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueOptionController.generated.h"

UCLASS()
class YADSP_API UDialogueOptionController : public UUserWidget
{
	GENERATED_BODY()

public: // Methods
	UDialogueOptionController(const FObjectInitializer& ObjectInitializer);
	virtual ~UDialogueOptionController() override {}

	void SetClickHandler(int Index, std::function<void(int)> ClickHandler);

	UFUNCTION()
	void OnButtonClicked();

	// Create Method
	static UDialogueOptionController* CreateInstance(APlayerController* PlayerController);

	// Properties
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
