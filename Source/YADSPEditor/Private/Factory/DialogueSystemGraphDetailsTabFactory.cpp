// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemGraphDetailsTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "UI/SDialogueGraphDetailsTab.h"

DialogueSystemGraphDetailsTabFactory::DialogueSystemGraphDetailsTabFactory(TSharedPtr<class DialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphDetailsTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Graph Details"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Details view for the dialogue graph"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Graph Details view"));
}

TSharedRef<SWidget> DialogueSystemGraphDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin();

	return SNew(SDialogueGraphDetailsTab, App);
}

FText DialogueSystemGraphDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Details view for the dialogue graph"));
}
