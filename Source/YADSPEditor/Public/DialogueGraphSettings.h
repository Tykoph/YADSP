// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueGraphSettings.generated.h"

UCLASS(Config=EditorPerProjectUserSettings)
class UDialogueGraphSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
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
	
private:
	void InitLanguageOptions();
	
	// Currently selected language for preview
	FString PreviewLanguage = TEXT("en-US");

	// Available language options for the dropdown
	TArray<TSharedPtr<FString>> LanguageOptions;
};
