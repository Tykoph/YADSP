// Copyright 2026 Tom Duby. All Rights Reserved.

#include "UI/SDialoguePropertiesTab.h"
#include "DialogueGraphEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

void SDialoguePropertiesTab::Construct(const FArguments& InArgs, TSharedPtr<DialogueGraphEditorApp> InApp)
{
	DialogueGraphApp = InApp;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	// Detail view args
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bSearchInitialKeyFocus = true;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.NotifyHook = nullptr;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bShowModifiedPropertiesOption = false;
	DetailsViewArgs.bShowScrollBar = false;

	const TSharedPtr<IDetailsView> SelectedNodeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SelectedNodeDetailsView->SetObject(nullptr);
	InApp->SetSelectedNodeDetailView(SelectedNodeDetailsView);

	ChildSlot
	[
		SelectedNodeDetailsView.ToSharedRef()
	];
}
