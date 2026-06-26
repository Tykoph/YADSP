// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemPreviewTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "UI/SDialoguePreviewTab.h"

DialogueSystemPreviewTabFactory::DialogueSystemPreviewTabFactory(TSharedPtr<class DialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphPreviewTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Preview"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Rich Text Preview for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Rich Text Preview view"));
}

TSharedRef<SWidget> DialogueSystemPreviewTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin();

	return SNew(SDialoguePreviewTab, App);
}

FText DialogueSystemPreviewTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Rich Text Preview for the selected dialogue node"));
}
