// Copyright 2026 Tom Duby. All Rights Reserved.

#include "UI/DialogueUIController.h"

#include "DialogueSubsystem.h"
#include "Fonts/FontMeasure.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"


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

void UDialogueUIController::IsTextWrapping(UTextBlock* InDialogueText, const FString& Text)
{
	const FSlateFontInfo FontInfo = InDialogueText->GetFont();
	const TSharedPtr<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	// Calculate Text Width
	if (FontMeasure.IsValid())
	{
		const FVector2D TextSize = FontMeasure->Measure(Text, FontInfo);
		const FVector2D ContainerText = InDialogueText->GetCachedGeometry().GetLocalSize();
		if (TextSize.X > ContainerText.X) {
			InDialogueText->SetJustification(ETextJustify::Left);
		}
		else {
			InDialogueText->SetJustification(ETextJustify::Center);
		}
	}
}

void UDialogueUIController::OnDialogueEnded()
{
	RemoveFromParent();
}
