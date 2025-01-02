#include "AssetTypeActions_DialogueGraph.h"
#include "CoreMinimal.h"
#include "DialogueGraph.h"
#include "DialogueGraphEditorApp.h"

FAssetTypeActions_DialogueGraph::FAssetTypeActions_DialogueGraph(EAssetTypeCategories::Type AssetCategory)
{
	_assetCategory = AssetCategory;
}

//  FAssetTypeActions_Base interface
FText FAssetTypeActions_DialogueGraph::GetName()  const
{
	return FText::FromString(TEXT("Dialogue Graph"));
}

FColor  FAssetTypeActions_DialogueGraph::GetTypeColor() const
{
	return FColor::FromHex("007fff");
}

UClass*  FAssetTypeActions_DialogueGraph::GetSupportedClass() const
{
	return UDialogueGraph::StaticClass();
}

uint32  FAssetTypeActions_DialogueGraph::GetCategories()
{
	return _assetCategory;
}

void  FAssetTypeActions_DialogueGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : InObjects)
	{
		UDialogueGraph* dialogueGraph = Cast<UDialogueGraph>(object);
		if (dialogueGraph != nullptr)
		{
			TSharedRef<DialogueGraphEditorApp> dialogueGraphEditorApp = MakeShareable(new DialogueGraphEditorApp());
			dialogueGraphEditorApp->InitEditor(Mode, EditWithinLevelEditor, dialogueGraph);
		}
	}
}