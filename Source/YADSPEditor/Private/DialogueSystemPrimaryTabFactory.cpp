#include "DialogueSystemPrimaryTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueSystem.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

DialogueSystemPrimaryTabFactory::DialogueSystemPrimaryTabFactory(const TSharedPtr<class DialogueGraphEditorApp>& App) : FWorkflowTabFactory(FName("GraphPrimaryTab"), App)
{
	DGApp = App;
	TabLabel = FText::FromString(TEXT("Primary"));
	ViewMenuDescription = FText::FromString(TEXT("Display a primary view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Primary view"));
}

TSharedRef<SWidget> DialogueSystemPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = DGApp.Pin();

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
