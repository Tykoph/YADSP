// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class YADSPEDITOR_API DialogueSystemAppMode : public FApplicationMode
{
public:
	DialogueSystemAppMode(TSharedPtr<class DialogueGraphEditorApp> App);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<DialogueGraphEditorApp> DGApp;
	FWorkflowAllowedTabSet Tabs;
};
