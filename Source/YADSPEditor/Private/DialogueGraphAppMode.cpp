#include "DialogueGraphAppMode.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueSystemPrimaryTabFactory.h"
#include "DialogueSystemPropertiesTabFactory.h"

DialogueGraphAppMode::DialogueGraphAppMode(TSharedPtr<class DialogueGraphEditorApp> App): FApplicationMode(TEXT("DialogueGraphAppMode"))
{
	_app = App;
	_tabs.RegisterFactory(MakeShareable(new DialogueSystemPrimaryTabFactory(App)));
	_tabs.RegisterFactory(MakeShareable(new DialogueSystemPropertiesTabFactory(App)));

	// Slate UI
	TabLayout = FTabManager::NewLayout("DialogueGraphAppMode_Layout_v1")
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
					->SetSizeCoefficient(0.75)
					->AddTab("GraphPrimaryTab", ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.25)
					->AddTab("GraphPropertyTab", ETabState::OpenedTab)
				)
		));
}

void DialogueGraphAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager)
{
	TSharedPtr<DialogueGraphEditorApp> App = _app.Pin();
	App->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void DialogueGraphAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void DialogueGraphAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
