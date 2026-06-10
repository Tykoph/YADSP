// Copyright 2026 Tom Duby. All Rights Reserved.

#include "UI/DialogueUIController.h"

#include "DialogueSubsystem.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlock.h"
#include "Fonts/FontMeasure.h"


void UDialogueUIController::NativeConstruct()
{
	Super::NativeConstruct();
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>();
	DialogueSubsystem->OnDialogueLineRequested.AddDynamic(this, &UDialogueUIController::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.AddDynamic(this, &UDialogueUIController::OnDialogueEnded);
}

void UDialogueUIController::NativeDestruct()
{
	Super::NativeDestruct();
	DialogueSubsystem->OnDialogueLineRequested.RemoveDynamic(this, &UDialogueUIController::UpdateDisplay);
	DialogueSubsystem->OnDialogueEnded.RemoveDynamic(this, &UDialogueUIController::OnDialogueEnded);
}

void UDialogueUIController::UpdateDisplay_Implementation(const FText& Text, const FText& Speaker, const TArray<FText>& Options)
{
	SpeakerName->SetText(Speaker);
	DialogueText->SetText(Text);
	IsTextWrapping(DialogueText, Text.ToString());
	
	ResponseBox->ClearChildren();
	DialogueOptionsWidgets.Empty();
	
	DialogueOptions = Options;
	DisplayDialogueOptions();
}

void UDialogueUIController::DisplayDialogueOptions()
{
	for (int i = 0; i < DialogueOptions.Num(); ++i) {
		DialogueOptionsWidgets.Add(CreateWidget(this, DialogueOptionClass));
		ResponseBox->AddChild(DialogueOptionsWidgets[i]);
		Cast<UDialogueOption>(DialogueOptionsWidgets[i])->SetDialogueOption(DialogueOptions[i], i);
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
