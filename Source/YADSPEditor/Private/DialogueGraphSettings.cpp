// Copyright 2026 Tom Duby. All Rights Reserved.

#include "DialogueGraphSettings.h"

UDialogueGraphSettings::UDialogueGraphSettings()
{
	InitLanguageOptions();
}

FString UDialogueGraphSettings::GetPreviewLanguage() const
{
	return PreviewLanguage;
}

void UDialogueGraphSettings::SetPreviewLanguage(const FString& NewLanguage)
{
	UDialogueGraphSettings* Settings = UDialogueGraphSettings::Get();
	Settings->PreviewLanguage = NewLanguage;
	Settings->SaveConfig();
	Settings->OnPreviewLanguageChanged.Broadcast(); 
}

const TArray<TSharedPtr<FString>>* UDialogueGraphSettings::GetLanguageOptions() const
{
	return &LanguageOptions;
}

void UDialogueGraphSettings::InitLanguageOptions()
{
	LanguageOptions.Add(MakeShared<FString>(TEXT("zh-Hans")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-150")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-US")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("fr")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("ja")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("pt-BR")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("es-ES")));
}
