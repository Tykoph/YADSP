// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueResponseWidget.h"

#include "DialogueSubsystem.h"
#include "YADSP.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"

void UDialogueResponseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueOption::NativeConstruct -> DialogueSubsystem not found"))
	}
}

void UDialogueResponseWidget::SetDialogueOption(const FText& InText, const int InIndex)
{
	ButtonIndex = InIndex;
	if (ResponseButtonText) {
		ResponseButtonText->SetText(InText);
	}
	
	if (ResponseButton) {
		ResponseButton->OnClicked.AddUniqueDynamic(this, &UDialogueResponseWidget::OnButtonClicked);
	}
}

void UDialogueResponseWidget::OnButtonClicked()
{
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueOption::OnButtonClicked -> DialogueSubsystem not found"))
		return;
	}
	
	DialogueSubsystem->OnOptionSelected.Broadcast(ButtonIndex);
}
