// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemPreviewTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "YADSP.h"
#include "UI/SDialoguePreviewTab.h"

FDialogueSystemPreviewTabFactory::FDialogueSystemPreviewTabFactory(TSharedPtr<class FDialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphPreviewTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Preview"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Rich Text Preview for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Rich Text Preview view"));
}

TSharedRef<SWidget> FDialogueSystemPreviewTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueGraphEditorApp> App = DialogueGraphApp.Pin();
	if (!App.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueSystemPreviewTabFactory::CreateTabBody -> App is invalid"));
		return SNew(SBox);
	}
	return SNew(SDialoguePreviewTab, App);
}

FText FDialogueSystemPreviewTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Rich Text Preview for the selected dialogue node"));
}
