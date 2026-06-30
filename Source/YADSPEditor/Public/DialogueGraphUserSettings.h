// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InputCoreTypes.h"
#include "Framework/Commands/InputChord.h"
#include "DialogueGraphUserSettings.generated.h"

USTRUCT()
struct FDialogueShortcut
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	FKey Key;

	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	bool bCtrl = false;

	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	bool bAlt = false;

	UPROPERTY(EditAnywhere, config, Category="Shortcut")
	bool bShift = false;

	FDialogueShortcut() {}
	FDialogueShortcut(const FKey& InKey) : Key(InKey) {}

	FInputChord GetInputChord() const
	{
		return FInputChord(Key, bShift, bCtrl, bAlt, false);
	}
};

DECLARE_MULTICAST_DELEGATE(FOnShortcutsChanged);

UCLASS(Config=EditorPerProjectUserSettings, meta=(DisplayName="YADSP"))
class UDialogueGraphUserSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	virtual FName GetContainerName() const override { return FName("Editor"); }
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	virtual FName GetSectionName() const override { return FName("YADSP"); }
	
	static UDialogueGraphUserSettings* Get()
	{
		return GetMutableDefault<UDialogueGraphUserSettings>();
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateTextNodeShortcut = FDialogueShortcut(EKeys::T);
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateBranchNodeShortcut = FDialogueShortcut(EKeys::B);
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateGameActionNodeShortcut = FDialogueShortcut(EKeys::A);
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateGoToNodeShortcut = FDialogueShortcut(EKeys::G);
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateLabelNodeShortcut = FDialogueShortcut(EKeys::L);
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut CreateEndNodeShortcut = FDialogueShortcut(EKeys::E);
	
	UPROPERTY(EditAnywhere, config, Category="Shortcuts")
	FDialogueShortcut DeleteNodeShortcut = FDialogueShortcut(EKeys::Delete);

	FOnShortcutsChanged OnShortcutsChanged;
};
