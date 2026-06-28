// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueWidget.h"

#include "DialogueSubsystem.h"
#include "YADSP.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlock.h"
#include "Fonts/FontMeasure.h"

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
		UE_LOG(LogYADSP, Error, TEXT("DialogueUIController::NativeDestruct -> DialogueSubsystem not found"))
		return;
	}
	DialogueSubsystem->OnDialogueLineRequested.RemoveDynamic(this, &UDialogueWidget::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.RemoveDynamic(this, &UDialogueWidget::OnDialogueEnded);
	DialogueSubsystem->OnBranchOptionsRequested.RemoveDynamic(this, &UDialogueWidget::OnBranchOptionsRequested);
	DialogueSubsystem->OnOptionSelected.RemoveDynamic(this, &UDialogueWidget::ClearDialogueOption);
}

void UDialogueWidget::UpdateDisplay_Implementation(const FText& InText, const FText& InSpeaker)
{
	if (SpeakerName == nullptr || DialogueText == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueUIController::UpdateDisplay_Implementation -> %s%s"),
			(SpeakerName ? TEXT("") : TEXT("WorkingAsset is Null. ")),
			(DialogueText ? TEXT("") : TEXT("WorkingGraphEditor is Null. "))
			);
		return;
	}
	
	SpeakerName->SetText(InSpeaker);
	DialogueText->SetText(InText);
	UpdateTextWrapping(DialogueText, InText.ToString());
}

void UDialogueWidget::OnBranchOptionsRequested(const TArray<FBranchOption>& InBranchOptions)
{	
	if (ResponseBox == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueUIController::UpdateDisplay_Implementation -> ResponseBox is null"));
		return;
	}
	
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
	
	if (DialogueOptionWidgetClass == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueUIController::OnBranchOptionsRequested -> DialogueOptionClass is nullptr"))
		return;
	}
	
	for (int i = 0; i < InBranchOptions.Num(); ++i) {
		DialogueOptionsWidgets.Add(CreateWidget(this, DialogueOptionWidgetClass));
		ResponseBox->AddChild(DialogueOptionsWidgets[i]);
		
		const auto DialogueOption = Cast<UDialogueResponseWidget>(DialogueOptionsWidgets[i]);
		if (DialogueOption == nullptr) {
			UE_LOG(LogYADSP, Error, TEXT("DialogueUIController::OnBranchOptionsRequested -> DialogueOption is nullptr"))
			continue;
		}
		
		DialogueOption->SetDialogueOption(InBranchOptions[i].DialogueText, i);
		DialogueOption->bIsValid = InBranchOptions[i].bExpressionIsValid;
		DialogueOption->OptionTooltip = InBranchOptions[i].Tooltip;
	}
}

void UDialogueWidget::UpdateTextWrapping(URichTextBlock* InDialogueTextBlock, const FString& InText)
{
	if (InDialogueTextBlock == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueUIController::IsTextWrapping -> DialogueTextBlock is nullptr"))
		return;
	}
	
	InDialogueTextBlock->ForceLayoutPrepass();
	const FSlateFontInfo FontInfo = InDialogueTextBlock->GetCurrentDefaultTextStyle().Font;
	const TSharedPtr<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	
	// Calculate Text Width
	if (FontMeasure.IsValid()) {
		const FVector2D TextSize = FontMeasure->Measure(InText, FontInfo);
		const FVector2D ContainerText = InDialogueTextBlock->GetCachedGeometry().GetLocalSize();
		if (TextSize.X > ContainerText.X) {
			InDialogueTextBlock->SetJustification(ETextJustify::Left);
		}
		else {
			InDialogueTextBlock->SetJustification(ETextJustify::Center);
		}
	}
	else {
		UE_LOG(LogYADSP, Warning, TEXT("UDialogueUIController::IsTextWrapping -> Font Measure is invalid"))
	}
}

void UDialogueWidget::OnDialogueEnded()
{
	RemoveFromParent();
}

void UDialogueWidget::ClearDialogueOption_Implementation(int Index)
{
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
}
