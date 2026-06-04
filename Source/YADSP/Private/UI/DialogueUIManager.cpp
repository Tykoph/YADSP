#include "UI/DialogueUIManager.h"

void UDialogueUIManager::BeginPlay()
{
	Super::BeginPlay();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	DialogueSubsystem->OnDialogueStarted.AddDynamic(this, &UDialogueUIManager::OnDialogueStarted);
	DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &UDialogueUIManager::OnDialogueEnded);
}

void UDialogueUIManager::OnDialogueStarted(UDialogueSystem* DialogueAsset, APlayerController* PC)
{
	if (DialogueUI != nullptr) return;
	DialogueUI = CreateWidget(PC, DialogueUIClass);
	DialogueUI->AddToViewport();
}

void UDialogueUIManager::OnDialogueEnded()
{
	DialogueUI = nullptr;
}
