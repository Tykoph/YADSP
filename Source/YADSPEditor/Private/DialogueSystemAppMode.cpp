// Copyright 2026 Tom Duby. All Rights Reserved.

#include "DialogueSystemAppMode.h"
#include "DialogueGraphEditorApp.h"
#include "Factory/DialogueSystemPrimaryTabFactory.h"
#include "Factory/DialogueSystemPropertiesTabFactory.h"
#include "Factory/DialogueSystemPreviewTabFactory.h"
#include "Factory/DialogueSystemGraphDetailsTabFactory.h"

DialogueSystemAppMode::DialogueSystemAppMode(TSharedPtr<class DialogueGraphEditorApp> App): FApplicationMode(TEXT("DialogueGraphAppMode"))
{
	DGApp = App;
	Tabs.RegisterFactory(MakeShareable(new DialogueSystemPrimaryTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new DialogueSystemPropertiesTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new DialogueSystemPreviewTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new DialogueSystemGraphDetailsTabFactory(App)));

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

void DialogueSystemAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	TSharedPtr<DialogueGraphEditorApp> App = DGApp.Pin();
	App->PushTabFactories(Tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void DialogueSystemAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void DialogueSystemAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
