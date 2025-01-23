// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewQuestController.Generated.h"

UCLASS()
class YADSP_API UNewQuestController : public UUserWidget
{
	GENERATED_BODY()

public: // Methods
	UNewQuestController(const FObjectInitializer& objectInitializer);

	virtual ~UNewQuestController() override
	{
	}

	// Create Method
	static UNewQuestController* CreateInstance(APlayerController* PlayerController);

	// Properties
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* QuestText = nullptr;
};

UCLASS()
class UNewQuestLoader : public UObject
{
	GENERATED_BODY()

public:
	UNewQuestLoader();
	virtual ~UNewQuestLoader() override {}

	UPROPERTY()
	UClass* WidgetTemplate = nullptr;
	static const inline FString WidgetPath = TEXT("/YADSP/WBP_NewQuest");
};
