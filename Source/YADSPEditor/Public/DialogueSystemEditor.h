// Copyright Tom Duby. All Rights Reserved.

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
	TSharedPtr<FSlateStyleSet> DGStyleSet = nullptr;
	
	TSharedPtr<struct FDialoguePinFactory> PinFactory = nullptr;
	TSharedPtr<struct FDialogueGraphNodeFactory> NodeFactory = nullptr;
};
