#include "DialogueSystemAction.h"
#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueGraphEditorApp.h"

FDialogueSystemAction::FDialogueSystemAction(EAssetTypeCategories::Type AssetCategory)
{
	_assetCategory = AssetCategory;
}

//  FAssetTypeActions_Base interface
FText FDialogueSystemAction::GetName()  const
{
	return FText::FromString(TEXT("Dialogue Graph"));
}

FColor  FDialogueSystemAction::GetTypeColor() const
{
	return FColor::FromHex("007fff");
}

UClass*  FDialogueSystemAction::GetSupportedClass() const
{
	return UDialogueSystem::StaticClass();
}

uint32  FDialogueSystemAction::GetCategories()
{
	return _assetCategory;
}

void  FDialogueSystemAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : InObjects)
	{
		UDialogueSystem* dialogueGraph = Cast<UDialogueSystem>(object);
		if (dialogueGraph != nullptr)
		{
			TSharedRef<DialogueGraphEditorApp> dialogueGraphEditorApp(new DialogueGraphEditorApp());
			dialogueGraphEditorApp->InitEditor(Mode, EditWithinLevelEditor, dialogueGraph);
		}
	}
}