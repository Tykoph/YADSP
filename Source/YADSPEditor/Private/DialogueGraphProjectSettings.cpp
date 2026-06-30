// Copyright Tom Duby. All Rights Reserved.

#include "DialogueGraphProjectSettings.h"

#include "YADSP.h"
#include "Engine/DataTable.h"
#include "Styling/SlateStyle.h"
#include "Components/RichTextBlock.h"
#include "Brushes/SlateImageBrush.h"

UDialogueGraphProjectSettings::UDialogueGraphProjectSettings()
{
	InitLanguageOptions();
}

const FString& UDialogueGraphProjectSettings::GetPreviewLanguage() const
{
	return PreviewLanguage;
}

void UDialogueGraphProjectSettings::SetPreviewLanguage(const FString& InNewLanguage)
{
	UDialogueGraphProjectSettings* Settings = UDialogueGraphProjectSettings::Get();
	Settings->PreviewLanguage = InNewLanguage;
	Settings->SaveConfig();
	Settings->OnPreviewLanguageChanged.Broadcast(); 
}

const TArray<TSharedPtr<FString>>* UDialogueGraphProjectSettings::GetLanguageOptions() const
{
	return &LanguageOptions;
}

TArray<FString> UDialogueGraphProjectSettings::GetStyleOption() const
{
	TArray<FString> Options;

	if (PreviewRichTextStyleSet) {
		TArray<FName> RowNames = PreviewRichTextStyleSet->GetRowNames();

		for (const FName& Name : RowNames) {
			Options.Add(Name.ToString());
		}
	}

	if (Options.Num() == 0) {
		Options.Add(TEXT("No Table Found"));
	}

	return Options;
}

void UDialogueGraphProjectSettings::InitLanguageOptions()
{
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-150")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-US")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("fr")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("ja")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("zh-Hans")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("pt-BR")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("es-ES")));
}

void UDialogueGraphProjectSettings::ResetCachedStyle()
{
	CachedStyleInstance.Reset();
	OnRichTextStyleChanged.Broadcast();
}

TSharedPtr<ISlateStyle> UDialogueGraphProjectSettings::GetRichTextStyleSet()
{
	if (!CachedStyleInstance.IsValid()) {
		CachedStyleInstance = MakeShareable(new FSlateStyleSet(TEXT("DialogueRichTextStyle")));
		
		UDataTable* Table = PreviewRichTextStyleSet.LoadSynchronous();
		if (Table == nullptr) {
			UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphSettings::GetRichTextStyleSet -> Table is not correctly initialized"))
		}
		BindToDataTable(Table);

		if (Table && Table->GetRowStruct()->IsChildOf(FRichTextStyleRow::StaticStruct())) {
			for (const auto& Entry : Table->GetRowMap()) {
				const FName SubStyleName = Entry.Key;
				const FRichTextStyleRow* RichTextStyle = reinterpret_cast<FRichTextStyleRow*>(Entry.Value);
				CachedStyleInstance->Set(SubStyleName, RichTextStyle->TextStyle);
			}
		}
	}
	return CachedStyleInstance;
}

void UDialogueGraphProjectSettings::BindToDataTable(UDataTable* Table)
{
	if (BoundDataTable.Get() != Table)
	{
		if (UDataTable* OldTable = BoundDataTable.Get()) {
			OldTable->OnDataTableChanged().RemoveAll(this);
		}

		BoundDataTable = Table;

		if (Table) {
			Table->OnDataTableChanged().AddUObject(this, &UDialogueGraphProjectSettings::ResetCachedStyle);
		}
	}
}

#if WITH_EDITOR
void UDialogueGraphProjectSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UDialogueGraphProjectSettings, PreviewRichTextStyleSet)) {
		UDataTable* Table = PreviewRichTextStyleSet.LoadSynchronous();
		if (Table == nullptr) {
			UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphSettings::PostEditChangeProperty -> Table is not correctly initialized"))
			return;
		}
		
		if (Table->GetRowStruct()->IsChildOf(FRichTextStyleRow::StaticStruct())) {
			for (const auto& Entry : Table->GetRowMap()) {
				const FName SubStyleName = Entry.Key;
				const FRichTextStyleRow* RichTextStyle = reinterpret_cast<FRichTextStyleRow*>(Entry.Value);
				if (!CachedStyleInstance.IsValid()) {
					UE_LOG(LogYADSP, Error, TEXT("UDialogueGraphSettings::PostEditChangeProperty -> CachedStyleInstance is not valid"))
					return;
				}
				CachedStyleInstance->Set(SubStyleName, RichTextStyle->TextStyle);
			}
		}
	}
}
#endif
