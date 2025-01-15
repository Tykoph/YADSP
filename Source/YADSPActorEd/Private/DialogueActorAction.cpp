#include "DialogueActorAction.h"
#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "DialogueActor.h"
#include "Toolkits/IToolkit.h"

DialogueActorAction::DialogueActorAction(EAssetTypeCategories::Type AssetCategory)
{
	_AssetCategory = AssetCategory;
}

//  FAssetTypeActions_Base interface
FText DialogueActorAction::GetName()  const
{
	return FText::FromString(TEXT("Dialogue Actor"));
}

FColor DialogueActorAction::GetTypeColor() const
{
	return FColor::FromHex("007fff");
}

UClass* DialogueActorAction::GetSupportedClass() const
{
	return UDialogueSystem::StaticClass();
}

uint32 DialogueActorAction::GetCategories()
{
	return _AssetCategory;
}

void DialogueActorAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects)
	{
		UDialogueSystem* DialogueActor = Cast<UDialogueSystem>(Object);
	}
}