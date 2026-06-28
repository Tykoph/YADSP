// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueUIManager.h"
#include "YADSP.h"

void UDialogueUIManager::BeginPlay()
{
	Super::BeginPlay();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueUIManager::BeginPlay -> DialogueSubsystem not found"))
		return;
	}
	DialogueSubsystem->OnDialogueStarted.AddDynamic(this, &UDialogueUIManager::OnDialogueStarted);
	DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &UDialogueUIManager::OnDialogueEnded);
}

void UDialogueUIManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueUIManager::EndPlay -> DialogueSubsystem not found"))
		return;
	}
	DialogueSubsystem->OnDialogueStarted.RemoveDynamic(this, &UDialogueUIManager::OnDialogueStarted);
	DialogueSubsystem->OnDialogueEnded.RemoveDynamic(this, &UDialogueUIManager::OnDialogueEnded);
}

void UDialogueUIManager::OnDialogueStarted(UDialogueSystem* InDialogueAsset, APlayerController* InPlayerController)
{
	if (DialogueWidgetClass == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("No DialogueWidget Class assigned"))
		return;
	}
	if (DialogueUI != nullptr) {
		UE_LOG(LogYADSP, Warning, TEXT("UDialogueUIManager::OnDialogueStarted -> DialogueWidget already exist"))
		return;
	}
	
	DialogueUI = CreateWidget(InPlayerController, DialogueWidgetClass);
	if (DialogueUI == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueUIManager::OnDialogueStarted -> DialogueWidget not correctly created"))
		return;
	}
	DialogueUI->AddToViewport();
}

void UDialogueUIManager::OnDialogueEnded()
{
	if (DialogueUI == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueUIManager::OnDialogueEnded -> DialogueUI not found"))
		return;
	}
	DialogueUI->RemoveFromParent();
	DialogueUI = nullptr;
}
