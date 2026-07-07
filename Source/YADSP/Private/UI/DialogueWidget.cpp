// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueWidget.h"

#include "DialogueSubsystem.h"
#include "YADSP.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlock.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueUIController::NativeConstruct -> DialogueSubsystem not found"))
		return;
	}
	DialogueSubsystem->OnDialogueLineRequested.AddDynamic(this, &UDialogueWidget::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &UDialogueWidget::OnDialogueEnded);
	DialogueSubsystem->OnBranchOptionsRequested.AddDynamic(this, &UDialogueWidget::OnBranchOptionsRequested);
	DialogueSubsystem->OnOptionSelected.AddDynamic(this, &UDialogueWidget::ClearDialogueOption);
}

void UDialogueWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueWidget::NativeDestruct -> DialogueSubsystem not found"))
		return;
	}
	DialogueSubsystem->OnDialogueLineRequested.RemoveDynamic(this, &UDialogueWidget::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.RemoveDynamic(this, &UDialogueWidget::OnDialogueEnded);
	DialogueSubsystem->OnBranchOptionsRequested.RemoveDynamic(this, &UDialogueWidget::OnBranchOptionsRequested);
	DialogueSubsystem->OnOptionSelected.RemoveDynamic(this, &UDialogueWidget::ClearDialogueOption);
}

void UDialogueWidget::UpdateDisplay_Implementation(const FText& InText, const FText& InSpeaker)
{
	if (SpeakerTextBlock == nullptr || DialogueTextBlock == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueWidget::UpdateDisplay_Implementation -> %s%s"),
			(SpeakerTextBlock ? TEXT("") : TEXT("WorkingAsset is Null. ")),
			(DialogueTextBlock ? TEXT("") : TEXT("WorkingGraphEditor is Null. "))
			);
		return;
	}
	
	SpeakerTextBlock->SetText(InSpeaker);
	DialogueTextBlock->SetText(InText);
}

void UDialogueWidget::OnBranchOptionsRequested(const TArray<FBranchOption>& InBranchOptions)
{	
	if (ResponseBox == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueWidget::UpdateDisplay_Implementation -> ResponseBox is null"));
		return;
	}
	
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
	
	if (DialogueOptionWidgetClass == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueWidget::OnBranchOptionsRequested -> DialogueOptionClass is nullptr"))
		return;
	}
	
	for (int i = 0; i < InBranchOptions.Num(); ++i) {
		DialogueOptionsWidgets.Add(CreateWidget(this, DialogueOptionWidgetClass));
		ResponseBox->AddChild(DialogueOptionsWidgets[i]);
		
		const auto DialogueOption = Cast<UDialogueResponseWidget>(DialogueOptionsWidgets[i]);
		if (DialogueOption == nullptr) {
			UE_LOG(LogYADSP, Error, TEXT("UDialogueWidget::OnBranchOptionsRequested -> DialogueOption is nullptr"))
			continue;
		}
		
		DialogueOption->SetDialogueOption(InBranchOptions[i].DialogueText, i);
		DialogueOption->bIsValid = InBranchOptions[i].bExpressionIsValid;
		DialogueOption->OptionTooltip = InBranchOptions[i].Tooltip;
	}
}

void UDialogueWidget::OnDialogueEnded()
{
	RemoveFromParent();
}

void UDialogueWidget::ClearDialogueOption_Implementation(int Index)
{
	if (ResponseBox == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueWidget::ClearDialogueOption_Implementation -> ResponseBox is null"));
		return;
	}
	
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
}
