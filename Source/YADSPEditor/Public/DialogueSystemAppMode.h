// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/**
 * Application mode for the Dialogue System Editor.
 */
class YADSPEDITOR_API FDialogueSystemAppMode : public FApplicationMode
{
public:
	/** Initializes the app mode with the owning editor. */
	explicit FDialogueSystemAppMode(TSharedPtr<class FDialogueGraphEditorApp> App);

	/** Registers layout tabs for this app mode. */
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<FDialogueGraphEditorApp> DGApp;
	FWorkflowAllowedTabSet Tabs;
};
