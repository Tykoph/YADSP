// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueSystemAction.h"
#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueGraphEditorApp.h"

DialogueSystemAction::DialogueSystemAction(EAssetTypeCategories::Type AssetCategory)
{
	AssetCategoryPtr = AssetCategory;
}

//  FAssetTypeActions_Base interface
FText DialogueSystemAction::GetName() const
{
	return FText::FromString(TEXT("Dialogue Graph"));
}

FColor DialogueSystemAction::GetTypeColor() const
{
	return FColor::FromHex("007fff");
}

UClass* DialogueSystemAction::GetSupportedClass() const
{
	return UDialogueSystem::StaticClass();
}

uint32 DialogueSystemAction::GetCategories()
{
	return AssetCategoryPtr;
}

void DialogueSystemAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects) {
		UDialogueSystem* DialogueGraph = Cast<UDialogueSystem>(Object);
		if (DialogueGraph != nullptr) {
			TSharedRef<DialogueGraphEditorApp> GraphEditorApp(new DialogueGraphEditorApp());
			GraphEditorApp->InitEditor(Mode, EditWithinLevelEditor, DialogueGraph);
		}
	}
}
