// Copyright Tom Duby. All Rights Reserved.

#include "DialogueSystemAppMode.h"
#include "DialogueGraphEditorApp.h"
#include "YADSP.h"
#include "Factory/DialogueSystemPrimaryTabFactory.h"
#include "Factory/DialogueSystemPropertiesTabFactory.h"
#include "Factory/DialogueSystemPreviewTabFactory.h"
#include "Factory/DialogueSystemGraphDetailsTabFactory.h"

FDialogueSystemAppMode::FDialogueSystemAppMode(TSharedPtr<class FDialogueGraphEditorApp> App): FApplicationMode(TEXT("DialogueGraphAppMode"))
{
	DGApp = App;
	Tabs.RegisterFactory(MakeShareable(new FDialogueSystemPrimaryTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new FDialogueSystemPropertiesTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new FDialogueSystemPreviewTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new FDialogueSystemGraphDetailsTabFactory(App)));

	// Slate UI
	TabLayout = FTabManager::NewLayout("DialogueGraphAppMode_Layout_v2")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.2)
					->AddTab("GraphDetailsTab", ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6)
					->AddTab("GraphPrimaryTab", ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.2)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.5)
						->AddTab("GraphPreviewTab", ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.5)
						->AddTab("GraphPropertyTab", ETabState::OpenedTab)
					)
				)
			));
}

void FDialogueSystemAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	const TSharedPtr<FDialogueGraphEditorApp> App = DGApp.Pin();
	if (!App.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueSystemAppMode::RegisterTabFactories -> App is invalid"));
		return;
	}
	App->PushTabFactories(Tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FDialogueSystemAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FDialogueSystemAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
