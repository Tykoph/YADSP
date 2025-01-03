#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

/**
 * 
 */
class YADSPEDITOR_API DialogueSystemPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	DialogueSystemPrimaryTabFactory(const TSharedPtr<class DialogueGraphEditorApp>& App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class DialogueGraphEditorApp> DGApp;
};
