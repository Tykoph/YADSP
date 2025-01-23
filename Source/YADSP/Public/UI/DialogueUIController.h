// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DialogueUIController.generated.h"

UCLASS()
class YADSP_API UDialogueUIController : public UUserWidget
{
	GENERATED_BODY()

public: // Methods
	UDialogueUIController(const FObjectInitializer& ObjectInitializer);
	virtual ~UDialogueUIController() override {}

	// Create Method
	static UDialogueUIController* CreateInstance(APlayerController* PlayerController);
	static void IsTextWrapping(UTextBlock* InDialogueText, const FString& Text);

	// Properties
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* SpeakerName = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DialogueText = nullptr;

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
