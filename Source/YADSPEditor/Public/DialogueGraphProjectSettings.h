// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "Components/RichTextBlockDecorator.h"
#include "DialogueGraphProjectSettings.generated.h"

/** Delegate fired when the preview language changes. */
DECLARE_MULTICAST_DELEGATE(FOnPreviewLanguageChanged);
/** Delegate fired when the rich text style changes. */
DECLARE_MULTICAST_DELEGATE(FOnRichTextStyleChanged);

/**
 * Project-wide settings for the Dialogue Graph.
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="YADSP"))
class UDialogueGraphProjectSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	// UDeveloperSettings interface
	virtual FName GetContainerName() const override { return FName("Project"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("YADSP"); }
	// /////////
	
	UDialogueGraphProjectSettings();
	
	/**
	 * Retrieves the mutable default instance of the settings.
	 * @return Pointer to the settings instance.
	 */
	static UDialogueGraphProjectSettings* Get()
	{
		return GetMutableDefault<UDialogueGraphProjectSettings>();
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/**
	 * Gets the currently selected language code for previewing localized text.
	 * @return The language code (e.g., "en-US", "fr").
	 */
	const FString& GetPreviewLanguage() const;

	/**
	 * Sets the language code for previewing localized text.
	 * Triggers a refresh of the graph editor to update text displays.
	 * @param InNewLanguage The new language code to use.
	 */
	static void SetPreviewLanguage(const FString& InNewLanguage);

	/**
	 * Gets the available language options for preview.
	 * @return Pointer to the array of language options.
	 */
	const TArray<TSharedPtr<FString>>* GetLanguageOptions() const;

	/**
	 * Retrieves the slate style set for rich text formatting.
	 * @return Shared pointer to the style set.
	 */
	TSharedPtr<ISlateStyle> GetRichTextStyleSet();
	
	/**
	 * Gets the available style options for dialogue presentation.
	 * @return Array of valid style names.
	 */
	UFUNCTION()
	TArray<FString> GetStyleOption() const;
	
	// Data table containing the rich text styles for previewing dialogue.
	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(RequiredAssetDataTags="RowStructure=/Script/UMG.RichTextStyleRow"))
	TSoftObjectPtr<UDataTable> PreviewRichTextStyleSet;

	// Decorators used when rendering rich text in the preview.
	UPROPERTY(EditAnywhere, config, Category="Preview")
	TArray<TSubclassOf<URichTextBlockDecorator>> PreviewDecorators;

	// Selected rich text style applied to the speaker name in preview.
	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(GetOptions="GetStyleOption"))
	FName SpeakerPreviewStyle;
	
	// Selected rich text style applied to the dialogue text in preview.
	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(GetOptions="GetStyleOption"))
	FName DialoguePreviewStyle;

	/** Invoked when the preview language has been changed. */
	FOnPreviewLanguageChanged OnPreviewLanguageChanged;

	/** Invoked when the active rich text style is changed. */
	FOnRichTextStyleChanged OnRichTextStyleChanged;
	
private:
	void BindToDataTable(UDataTable* Table);
	
	void InitLanguageOptions();
	
	void ResetCachedStyle();
	
	TSharedPtr<FSlateStyleSet> CachedStyleInstance;
	
	TWeakObjectPtr<UDataTable> BoundDataTable;
	
	// Currently selected language for preview
	FString PreviewLanguage = TEXT("en-US");
	// Available language options for the dropdown
	TArray<TSharedPtr<FString>> LanguageOptions;
	
};
