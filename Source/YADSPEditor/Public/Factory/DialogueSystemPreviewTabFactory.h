// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

/**
 * Factory responsible for creating the preview tab in the dialogue editor.
 */
class YADSPEDITOR_API FDialogueSystemPreviewTabFactory : public FWorkflowTabFactory
{
public:
	explicit FDialogueSystemPreviewTabFactory(TSharedPtr<class FDialogueGraphEditorApp> App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FDialogueGraphEditorApp> DialogueGraphApp;
};
