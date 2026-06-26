// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueOption.h"

#include "DialogueSubsystem.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"

void UDialogueOption::NativeConstruct()
{
	Super::NativeConstruct();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
}

void UDialogueOption::SetDialogueOption(const FText& Text, const int Index)
{
	ButtonIndex = Index;
	
	ResponseButtonText->SetText(Text);
	
	ResponseButton->OnClicked.AddUniqueDynamic(this, &UDialogueOption::OnButtonClicked);
}

void UDialogueOption::OnButtonClicked()
{
	DialogueSubsystem->OnOptionSelected.Broadcast(ButtonIndex);
}
