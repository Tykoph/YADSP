// Copyright Tom Duby. All Rights Reserved.

#include "DialogueGraphSettings.h"
#include "Engine/DataTable.h"
#include "Styling/SlateStyle.h"
#include "Components/RichTextBlock.h"
#include "Brushes/SlateImageBrush.h"

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

TArray<FString> UDialogueGraphSettings::GetStyleOption() const
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

void UDialogueGraphSettings::InitLanguageOptions()
{
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-150")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-US")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("fr")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("ja")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("zh-Hans")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("pt-BR")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("es-ES")));
}

void UDialogueGraphSettings::ResetCachedStyle()
{
	CachedStyleInstance.Reset();
	OnRichTextStyleChanged.Broadcast();
}

TSharedPtr<ISlateStyle> UDialogueGraphSettings::GetRichTextStyleSet()
{
	if (!CachedStyleInstance.IsValid()) {
		CachedStyleInstance = MakeShareable(new FSlateStyleSet(TEXT("DialogueRichTextStyle")));
		
		UDataTable* Table = PreviewRichTextStyleSet.LoadSynchronous();
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

void UDialogueGraphSettings::BindToDataTable(UDataTable* Table)
{
	if (BoundDataTable.Get() != Table)
	{
		if (UDataTable* OldTable = BoundDataTable.Get()) {
			OldTable->OnDataTableChanged().RemoveAll(this);
		}

		BoundDataTable = Table;

		if (Table) {
			Table->OnDataTableChanged().AddUObject(this, &UDialogueGraphSettings::ResetCachedStyle);
		}
	}
}

#if WITH_EDITOR
void UDialogueGraphSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UDialogueGraphSettings, PreviewRichTextStyleSet)) {
		UDataTable* Table = PreviewRichTextStyleSet.LoadSynchronous();

		if (Table && Table->GetRowStruct()->IsChildOf(FRichTextStyleRow::StaticStruct())) {
			for (const auto& Entry : Table->GetRowMap()) {
				const FName SubStyleName = Entry.Key;
				const FRichTextStyleRow* RichTextStyle = reinterpret_cast<FRichTextStyleRow*>(Entry.Value);
				CachedStyleInstance->Set(SubStyleName, RichTextStyle->TextStyle);
			}
		}
	}
}
#endif
