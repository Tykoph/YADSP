// Copyright 2025 Tom Duby. All Rights Reserved.

#include "UI/DialogueOptionController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/NewQuestController.h"

UDialogueOptionController::UDialogueOptionController(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UDialogueOptionController::SetClickHandler(int Index, std::function<void(int)> ClickHandler)
{
	ButtonIndex = Index;
	OnClickHandler = ClickHandler;

	ResponseButton->OnClicked.AddUniqueDynamic(this, &UDialogueOptionController::OnButtonClicked);
}

void UDialogueOptionController::OnButtonClicked()
{
	if (OnClickHandler)
	{
		OnClickHandler(ButtonIndex);
	}
}

UDialogueOptionController* UDialogueOptionController::CreateInstance(APlayerController* PlayerController)
{
	UDialogueOptionLoader* Loader = NewObject<UDialogueOptionLoader>(PlayerController);
	return Cast<UDialogueOptionController>(CreateWidget(PlayerController, Loader->WidgetTemplate));
}

UDialogueOptionLoader::UDialogueOptionLoader()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetTemplateFinder(*WidgetPath);
	WidgetTemplate = WidgetTemplateFinder.Class;
}
