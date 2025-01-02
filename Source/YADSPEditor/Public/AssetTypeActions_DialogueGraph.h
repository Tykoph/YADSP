#pragma once
#include "AssetTypeActions_Base.h"
#include "CoreMinimal.h"


class FAssetTypeActions_DialogueGraph : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_DialogueGraph(EAssetTypeCategories::Type AssetCategory);

	//  FAssetTypeActions_Base interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

private:
	EAssetTypeCategories::Type _assetCategory;
};
