// Copyright Tom Duby. All Rights Reserved.

#include "Factory/DialogueSystemPropertiesTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "YADSP.h"
#include "UI/SDialoguePropertiesTab.h"

FDialogueSystemPropertiesTabFactory::FDialogueSystemPropertiesTabFactory(TSharedPtr<class FDialogueGraphEditorApp> App) :
	FWorkflowTabFactory(FName("GraphPropertyTab"), App)
{
	DialogueGraphApp = App;
	TabLabel = FText::FromString(TEXT("Node Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Node Properties view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Node Properties view"));
}

TSharedRef<SWidget> FDialogueSystemPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueGraphEditorApp> App = DialogueGraphApp.Pin();
	if (!App.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueSystemPrimaryTabFactory::CreateTabBody -> App is invalid"));
		return SNew(SBox);
	}
	
	return SNew(SDialoguePropertiesTab, App);
}

FText FDialogueSystemPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Properties view for the dialogue graph editor"));
}
