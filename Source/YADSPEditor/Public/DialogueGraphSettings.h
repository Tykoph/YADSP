// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueGraphSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings, meta=(DisplayName="YADSP"))
class UDialogueGraphSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override { return FName("Project"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("YADSP"); }

	UDialogueGraphSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
public:
	DECLARE_MULTICAST_DELEGATE(FOnPreviewLanguageChanged);
	FOnPreviewLanguageChanged OnPreviewLanguageChanged;

	DECLARE_MULTICAST_DELEGATE(FOnRichTextStyleChanged);
	FOnRichTextStyleChanged OnRichTextStyleChanged;
	
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
	
	TWeakObjectPtr<UDataTable> BoundDataTable;
	void BindToDataTable(UDataTable* Table);
	
	void InitLanguageOptions();
	
	 void ResetCachedStyle();
	
	// Currently selected language for preview
	FString PreviewLanguage = TEXT("en-US");

	// Available language options for the dropdown
	TArray<TSharedPtr<FString>> LanguageOptions;
	
};
