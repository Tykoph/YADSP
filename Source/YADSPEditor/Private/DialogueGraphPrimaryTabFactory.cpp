#include "DialogueGraphPrimaryTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "CoreMinimal.h"
#include "DialogueGraph.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

DialogueGraphPrimaryTabFactory::DialogueGraphPrimaryTabFactory(const TSharedPtr<class DialogueGraphEditorApp>& App) : FWorkflowTabFactory(FName("GraphPrimaryTab"), App)
{
	DGApp = App;
	TabLabel = FText::FromString(TEXT("Primary"));
	ViewMenuDescription = FText::FromString(TEXT("Display a primary view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Primary view"));
}

TSharedRef<SWidget> DialogueGraphPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	const TSharedPtr<DialogueGraphEditorApp> App = DGApp.Pin();

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SGraphEditor)
				.IsEditable(true)
				.GraphToEdit(App->GetGraphEditor())
		];
}

FText DialogueGraphPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A primary view for the dialogue graph editor"));
}
