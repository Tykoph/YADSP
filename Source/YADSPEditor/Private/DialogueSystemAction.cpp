// Copyright Tom Duby. All Rights Reserved.

#include "DialogueSystemAction.h"
#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueGraphEditorApp.h"
#include "YADSP.h"

FDialogueSystemAction::FDialogueSystemAction(EAssetTypeCategories::Type InAssetCategory)
{
	AssetCategory = InAssetCategory;
}

//  FAssetTypeActions_Base interface
FText FDialogueSystemAction::GetName() const
{
	return FText::FromString(TEXT("Dialogue Graph"));
}

FColor FDialogueSystemAction::GetTypeColor() const
{
	return FColor::FromHex("007fff");
}

UClass* FDialogueSystemAction::GetSupportedClass() const
{
	return UDialogueSystem::StaticClass();
}

uint32 FDialogueSystemAction::GetCategories()
{
	return AssetCategory;
}

void FDialogueSystemAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects) {
		// Cast the object to dialogue system and initialize the editor if valid
		UDialogueSystem* DialogueGraph = Cast<UDialogueSystem>(Object);
		
		if (DialogueGraph == nullptr) {
			UE_LOG(LogYADSP, Error, TEXT("DialogueSystemAction::OpenAssetEditor -> Dialogue System is nullptr"))
			return;
		}
		
		const TSharedRef<FDialogueGraphEditorApp> GraphEditorApp(new FDialogueGraphEditorApp());
		GraphEditorApp->InitEditor(Mode, EditWithinLevelEditor, DialogueGraph);
	}
}
