// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"


/**
 * Asset type actions for Dialogue System assets.
 * Handles the registration and customization of dialogue graph assets in the Unreal Editor.
 */
class YADSPEDITOR_API FDialogueSystemAction : public FAssetTypeActions_Base
{
public:
	FDialogueSystemAction(EAssetTypeCategories::Type InAssetCategory);
	
	//  FAssetTypeActions_Base interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

	/**
	 * Opens the dialogue graph editor for the specified asset objects.
	 * @param InObjects Array of objects to edit (should be UDialogueSystem instances)
	 * @param EditWithinLevelEditor Level editor toolkit host, if editing within level editor
	 */
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
