// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class YADSPEDITOR_API FDialogueSystemAppMode : public FApplicationMode
{
public:
	FDialogueSystemAppMode(TSharedPtr<class FDialogueGraphEditorApp> App);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<FDialogueGraphEditorApp> DGApp;
	FWorkflowAllowedTabSet Tabs;
};
