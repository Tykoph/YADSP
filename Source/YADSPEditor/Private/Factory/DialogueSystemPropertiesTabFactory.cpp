// Copyright 2026 Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemPropertiesTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "UI/SDialoguePropertiesTab.h"

DialogueSystemPropertiesTabFactory::DialogueSystemPropertiesTabFactory(TSharedPtr<class DialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphPropertyTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Node Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Node Properties view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Node Properties view"));
}

TSharedRef<SWidget> DialogueSystemPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin();

	return SNew(SDialoguePropertiesTab, App);
}

FText DialogueSystemPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Properties view for the dialogue graph editor"));
}
