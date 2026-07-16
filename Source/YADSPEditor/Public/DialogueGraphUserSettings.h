// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InputCoreTypes.h"
#include "Framework/Commands/InputChord.h"
#include "DialogueGraphUserSettings.generated.h"

/**
 * Represents a customizable keyboard shortcut for a dialogue graph action.
 */
USTRUCT()
struct FDialogueShortcut
{
	GENERATED_BODY()

	// The primary key for the shortcut.
	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	// ReSharper disable once UnrealHeaderToolError
	FKey Key;

	// Whether the Ctrl key must be held.
	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	bool bCtrl = false;

	// Whether the Alt key must be held.
	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	bool bAlt = false;

	// Whether the Shift key must be held.
	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	bool bShift = false;

	FDialogueShortcut() {}
	explicit FDialogueShortcut(const FKey& InKey) : Key(InKey) {}

	/**
	 * Converts the shortcut into an Unreal input chord.
	 * @return The resulting input chord.
	 */
	FInputChord GetInputChord() const
	{
		return FInputChord(Key, bShift, bCtrl, bAlt, false);
	}
};

/** Delegate fired when the user's shortcuts have been modified. */
DECLARE_MULTICAST_DELEGATE(FOnShortcutsChanged);

/**
 * Editor preferences and user settings for the Dialogue Graph.
 */
UCLASS(Config=EditorPerProjectUserSettings, meta=(DisplayName="YADSP"))
class UDialogueGraphUserSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	// UDeveloperSettings interface
	virtual FName GetContainerName() const override { return FName("Editor"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("YADSP"); }
	// /////////

	/**
	 * Retrieves the mutable default instance of the user settings.
	 * @return Pointer to the user settings instance.
	 */
	static UDialogueGraphUserSettings* Get()
	{
		return GetMutableDefault<UDialogueGraphUserSettings>();
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Shortcut to create a text node.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateTextNodeShortcut = FDialogueShortcut(EKeys::T);
	// Shortcut to create a branch node.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateBranchNodeShortcut = FDialogueShortcut(EKeys::B);
	// Shortcut to create a game action node.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateGameActionNodeShortcut = FDialogueShortcut(EKeys::A);
	// Shortcut to create a GoTo node.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateGoToNodeShortcut = FDialogueShortcut(EKeys::G);
	// Shortcut to create a label node.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateLabelNodeShortcut = FDialogueShortcut(EKeys::L);
	// Shortcut to create an end node.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateEndNodeShortcut = FDialogueShortcut(EKeys::E);
	
	// Shortcut to delete selected nodes.
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut DeleteNodeShortcut = FDialogueShortcut(EKeys::Delete);
	
	// Whether to visually display rich text styling tags in the editor.
	UPROPERTY(EditAnywhere, config, Category="Preferences")
	bool bDisplayRichTextFlags = false;
	
	// The background color of the preview tab
	UPROPERTY(EditAnywhere, config, Category="Preferences")
	FLinearColor PreviewBackgroundColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
	
	/** Invoked when shortcut settings are modified. */
	FOnShortcutsChanged OnShortcutsChanged;
};
