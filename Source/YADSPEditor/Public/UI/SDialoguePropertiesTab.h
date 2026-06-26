// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphEditorApp.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class YADSPEDITOR_API SDialoguePropertiesTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialoguePropertiesTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<DialogueGraphEditorApp> InApp);

private:
	TWeakPtr<DialogueGraphEditorApp> DialogueGraphApp;
};
