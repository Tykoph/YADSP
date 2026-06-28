// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemGraphDetailsTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "YADSP.h"
#include "UI/SDialogueGraphDetailsTab.h"

FDialogueSystemGraphDetailsTabFactory::FDialogueSystemGraphDetailsTabFactory(TSharedPtr<class FDialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphDetailsTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Graph Details"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Details view for the dialogue graph"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Graph Details view"));
}

TSharedRef<SWidget> FDialogueSystemGraphDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueGraphEditorApp> App = DialogueGraphApp.Pin();
	if (!App.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueSystemGraphDetailsTabFactory::CreateTabBody -> App is invalid"));
		return SNew(SBox);
	}
	return SNew(SDialogueGraphDetailsTab, App);
}

FText FDialogueSystemGraphDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Details view for the dialogue graph"));
}
