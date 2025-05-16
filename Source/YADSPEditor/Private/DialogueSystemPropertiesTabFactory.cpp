// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueSystemPropertiesTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueSystem.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

DialogueSystemPropertiesTabFactory::DialogueSystemPropertiesTabFactory(TSharedPtr<class DialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphPropertyTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Properties view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Properties view"));
}

TSharedRef<SWidget> DialogueSystemPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
		TEXT("PropertyEditor"));

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

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(App->GetDialogueGraph());

	TSharedPtr<IDetailsView> SelectedNodeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SelectedNodeDetailsView->SetObject(nullptr);
	App->SetSelectedNodeDetailView(SelectedNodeDetailsView);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			SelectedNodeDetailsView.ToSharedRef()
		];
}

FText DialogueSystemPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Properties view for the dialogue graph editor"));
}
