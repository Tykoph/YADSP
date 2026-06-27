// Copyright Tom Duby. All Rights Reserved.

#include "UI/DialogueUIController.h"

#include "DialogueSubsystem.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlock.h"
#include "Fonts/FontMeasure.h"

void UDialogueUIController::NativeConstruct()
{
	Super::NativeConstruct();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	if (DialogueSubsystem == nullptr) 
		return;
	DialogueSubsystem->OnDialogueLineRequested.AddDynamic(this, &UDialogueUIController::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &UDialogueUIController::OnDialogueEnded);
	DialogueSubsystem->OnBranchOptionsRequested.AddDynamic(this, &UDialogueUIController::OnBranchOptionsRequested);
	DialogueSubsystem->OnOptionSelected.AddDynamic(this, &UDialogueUIController::ClearDialogueOption);
}

void UDialogueUIController::NativeDestruct()
{
	Super::NativeDestruct();
	if (DialogueSubsystem == nullptr) 
		return;
	DialogueSubsystem->OnDialogueLineRequested.RemoveDynamic(this, &UDialogueUIController::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.RemoveDynamic(this, &UDialogueUIController::OnDialogueEnded);
	DialogueSubsystem->OnBranchOptionsRequested.RemoveDynamic(this, &UDialogueUIController::OnBranchOptionsRequested);
	DialogueSubsystem->OnOptionSelected.RemoveDynamic(this, &UDialogueUIController::ClearDialogueOption);
}

void UDialogueUIController::UpdateDisplay_Implementation(const FText& Text, const FText& Speaker)
{
	SpeakerName->SetText(Speaker);
	DialogueText->SetText(Text);
	IsTextWrapping(DialogueText, Text.ToString());
}

void UDialogueUIController::OnBranchOptionsRequested(const TArray<FBranchOption>& BranchOptions)
{	
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
	
	for (int i = 0; i < BranchOptions.Num(); ++i) {
		DialogueOptionsWidgets.Add(CreateWidget(this, DialogueOptionClass));
		ResponseBox->AddChild(DialogueOptionsWidgets[i]);
		
		const auto DialogueOption = Cast<UDialogueOption>(DialogueOptionsWidgets[i]);
		DialogueOption->SetDialogueOption(BranchOptions[i].DialogueText, i);
		DialogueOption->bIsValid = BranchOptions[i].bExpressionIsValid;
		DialogueOption->OptionTooltip = BranchOptions[i].Tooltip;
	}
}

void UDialogueUIController::IsTextWrapping(URichTextBlock* DialogueTextBlock, const FString& Text)
{
	const FSlateFontInfo FontInfo = DialogueTextBlock->GetCurrentDefaultTextStyle().Font;
	const TSharedPtr<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	
	// Calculate Text Width
	if (FontMeasure.IsValid()) {
		const FVector2D TextSize = FontMeasure->Measure(Text, FontInfo);
		const FVector2D ContainerText = DialogueTextBlock->GetCachedGeometry().GetLocalSize();
		if (TextSize.X > ContainerText.X) {
			DialogueTextBlock->SetJustification(ETextJustify::Left);
		}
		else {
			DialogueTextBlock->SetJustification(ETextJustify::Center);
		}
	}
}

void UDialogueUIController::OnDialogueEnded()
{
	RemoveFromParent();
}

void UDialogueUIController::ClearDialogueOption_Implementation(int Index)
{
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
}
