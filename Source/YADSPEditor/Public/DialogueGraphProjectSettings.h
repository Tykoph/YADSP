// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "Components/RichTextBlockDecorator.h"
#include "DialogueGraphProjectSettings.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPreviewLanguageChanged);
DECLARE_MULTICAST_DELEGATE(FOnRichTextStyleChanged);

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="YADSP"))
class UDialogueGraphProjectSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override { return FName("Project"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("YADSP"); }

	UDialogueGraphProjectSettings();
	
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

	const TArray<TSharedPtr<FString>>* GetLanguageOptions() const;

	TSharedPtr<ISlateStyle> GetRichTextStyleSet();
	
	UFUNCTION()
	TArray<FString> GetStyleOption() const;
	
	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(RequiredAssetDataTags="RowStructure=/Script/UMG.RichTextStyleRow"))
	TSoftObjectPtr<UDataTable> PreviewRichTextStyleSet;

	UPROPERTY(EditAnywhere, config, Category="Preview")
	TArray<TSubclassOf<URichTextBlockDecorator>> PreviewDecorators;

	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(GetOptions="GetStyleOption"))
	FName SpeakerPreviewStyle;
	
	UPROPERTY(EditAnywhere, config, Category="Preview", meta=(GetOptions="GetStyleOption"))
	FName DialoguePreviewStyle;
		
	FOnPreviewLanguageChanged OnPreviewLanguageChanged;

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
