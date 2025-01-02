#include "DialogueGraphPropertiesTabFactory.h"
#include "DialogueGraphEditorApp.h"
#include "CoreMinimal.h"
#include "DialogueGraph.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

DialogueGraphPropertiesTabFactory::DialogueGraphPropertiesTabFactory(TSharedPtr<class DialogueGraphEditorApp> App) : FWorkflowTabFactory(FName("GraphPropertyTab"), App)
{
	_app = App;
	TabLabel = FText::FromString(TEXT("Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Display a Properties view for the dialogue graph editor"));
	ViewMenuTooltip = FText::FromString(TEXT("Show the Properties view"));
}

TSharedRef<SWidget> DialogueGraphPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<DialogueGraphEditorApp> App = _app.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	// Detail view args
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bSearchInitialKeyFocus = true;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.NotifyHook = nullptr;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bShowModifiedPropertiesOption = false;
	DetailsViewArgs.bShowScrollBar = false;

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(App->GetDialogueGraph());

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}

FText DialogueGraphPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A Properties view for the dialogue graph editor"));
}
