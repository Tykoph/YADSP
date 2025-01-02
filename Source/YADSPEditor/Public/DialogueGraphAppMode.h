#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/**
 * 
 */
class YADSPEDITOR_API DialogueGraphAppMode : public FApplicationMode
{
public:
	DialogueGraphAppMode(TSharedPtr<class DialogueGraphEditorApp> App);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual  void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class DialogueGraphEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};