// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"
#include "IAssetTools.h"
#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FDialogueSystemEditorModule : public IModuleInterface
{
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
	EAssetTypeCategories::Type DialogueGraphAssetCategory = EAssetTypeCategories::None;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	template <class T>
	void Internal_RegisterTypeActions(const FString& Name)
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		const auto Action = MakeShared<T>(DialogueGraphAssetCategory, FText::FromString(Name));
		RegisteredAssetTypeActions.Emplace(Action);
		AssetTools.RegisterAssetTypeActions(Action);
	}

	TSharedPtr<FSlateStyleSet> DGStyleSet = nullptr;
	TSharedPtr<struct FDialoguePinFactory> PinFactory = nullptr;
};
