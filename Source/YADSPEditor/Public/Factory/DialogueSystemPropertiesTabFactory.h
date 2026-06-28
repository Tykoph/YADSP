// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class YADSPEDITOR_API FDialogueSystemPropertiesTabFactory : public FWorkflowTabFactory
{
public:
	FDialogueSystemPropertiesTabFactory(TSharedPtr<class FDialogueGraphEditorApp> App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class FDialogueGraphEditorApp> DialogueGraphApp;
};
