// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueSystemPrimaryTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"

DialogueSystemPrimaryTabFactory::DialogueSystemPrimaryTabFactory(const TSharedPtr<class DialogueGraphEditorApp>& App) : FWorkflowTabFactory(FName("GraphPrimaryTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Primary"));
	ViewMenuDescription = FText::FromString(TEXT("Display a primary view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Primary view"));
}

TSharedRef<SWidget> DialogueSystemPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged.BindRaw(App.Get(), &DialogueGraphEditorApp::OnGraphSelectionChanged);

	TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
		.IsEditable(true)
		.GraphEvents(GraphEvents)
		.GraphToEdit(App->GetGraphEditor());
	App->SetWorkingGraphUi(GraphEditor);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			GraphEditor.ToSharedRef()
		];
}

FText DialogueSystemPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A primary view for the dialogue graph editor"));
}
