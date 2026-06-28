// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemPrimaryTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "YADSP.h"

FDialogueSystemPrimaryTabFactory::FDialogueSystemPrimaryTabFactory(const TSharedPtr<class FDialogueGraphEditorApp>& App) : FWorkflowTabFactory(FName("GraphPrimaryTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Primary"));
	ViewMenuDescription = FText::FromString(TEXT("Display a primary view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Primary view"));
}

TSharedRef<SWidget> FDialogueSystemPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	const TSharedPtr<FDialogueGraphEditorApp> App = DialogueGraphApp.Pin();
	if (!App.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueSystemPrimaryTabFactory::CreateTabBody -> App is invalid"));
		return SNew(SBox);
	}
	
	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged.BindRaw(App.Get(), &FDialogueGraphEditorApp::OnGraphSelectionChanged);

	const TSharedPtr<SGraphEditor> GraphEditor =
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

FText FDialogueSystemPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A primary view for the dialogue graph editor"));
}
