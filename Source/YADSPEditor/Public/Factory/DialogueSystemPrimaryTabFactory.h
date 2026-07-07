// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

/**
 * Factory responsible for creating the primary graph tab in the dialogue editor.
 */
class YADSPEDITOR_API FDialogueSystemPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	explicit FDialogueSystemPrimaryTabFactory(const TSharedPtr<class FDialogueGraphEditorApp>& App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FDialogueGraphEditorApp> DialogueGraphApp;
};
