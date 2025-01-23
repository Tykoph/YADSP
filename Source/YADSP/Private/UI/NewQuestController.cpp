// Copyright 2025 Tom Duby. All Rights Reserved.

#include "UI/NewQuestController.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

UNewQuestController::UNewQuestController(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

UNewQuestController* UNewQuestController::CreateInstance(APlayerController* PlayerController)
{
	UNewQuestLoader* Loader = NewObject<UNewQuestLoader>(PlayerController);
	return Cast<UNewQuestController>(CreateWidget(PlayerController, Loader->WidgetTemplate));
}

UNewQuestLoader::UNewQuestLoader()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetTemplateFinder(*WidgetPath);
	WidgetTemplate = WidgetTemplateFinder.Class;
}
