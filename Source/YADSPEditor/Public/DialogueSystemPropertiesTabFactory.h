#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class YADSPEDITOR_API DialogueSystemPropertiesTabFactory : public FWorkflowTabFactory
{
public:
	DialogueSystemPropertiesTabFactory(TSharedPtr<class DialogueGraphEditorApp> App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class DialogueGraphEditorApp> DGApp;
};
