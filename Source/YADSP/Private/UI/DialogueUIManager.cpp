// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueUIManager.h"
#include "YADSP.h"

void UDialogueUIManager::BeginPlay()
{
	Super::BeginPlay();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	if (DialogueSubsystem == nullptr) 
		return;
	DialogueSubsystem->OnDialogueStarted.AddDynamic(this, &UDialogueUIManager::OnDialogueStarted);
	DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &UDialogueUIManager::OnDialogueEnded);
}

void UDialogueUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (DialogueSubsystem) {
		DialogueSubsystem->OnDialogueStarted.RemoveDynamic(this, &UDialogueUIManager::OnDialogueStarted);
		DialogueSubsystem->OnDialogueEnded.RemoveDynamic(this, &UDialogueUIManager::OnDialogueEnded);
	}
}

void UDialogueUIManager::OnDialogueStarted(UDialogueSystem* DialogueAsset, APlayerController* PC)
{
	if (DialogueUIClass == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("No Dialogue UI Class assigned"))
		return;
	}
	if (DialogueUI != nullptr) return;
	DialogueUI = CreateWidget(PC, DialogueUIClass);
	DialogueUI->AddToViewport();
}

void UDialogueUIManager::OnDialogueEnded()
{
	DialogueUI = nullptr;
}
