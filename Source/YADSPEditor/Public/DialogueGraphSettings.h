// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueGraphSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, meta=(DisplayName="Dialogue Graph Settings"))
class UDialogueGraphSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override { return FName("Project"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("Dialogue Graph Settings"); }

	UDialogueGraphSettings();
	
public:
	DECLARE_MULTICAST_DELEGATE(FOnPreviewLanguageChanged);
	FOnPreviewLanguageChanged OnPreviewLanguageChanged;
	
	/**
	 * Gets the currently selected language code for previewing localized text.
	 * @return The language code (e.g., "en-US", "fr").
	 */
	FString GetPreviewLanguage() const;

	/**
	 * Sets the language code for previewing localized text.
	 * Triggers a refresh of the graph editor to update text displays.
	 * @param NewLanguage The new language code to use.
	 */
	static void SetPreviewLanguage(const FString& NewLanguage);

	const TArray<TSharedPtr<FString>>* GetLanguageOptions() const;
	
	static UDialogueGraphSettings* Get()
	{
		return GetMutableDefault<UDialogueGraphSettings>();
	}
	
	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(RequiredAssetDataTags="RowStructure=/Script/UMG.RichTextStyleRow"))
	TSoftObjectPtr<UDataTable> PreviewRichTextStyleSet;

	UPROPERTY(EditAnywhere, config, Category="Preview")
	TArray<TSubclassOf<class URichTextBlockDecorator>> PreviewDecorators;

	TSharedPtr<ISlateStyle> GetRichTextStyleSet();

private:
	TSharedPtr<FSlateStyleSet> CachedStyleInstance;
	
	void InitLanguageOptions();
	
	// Currently selected language for preview
	FString PreviewLanguage = TEXT("en-US");

	// Available language options for the dropdown
	TArray<TSharedPtr<FString>> LanguageOptions;
	
};
