// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphEditorApp.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class YADSPEDITOR_API SDialogueGraphDetailsTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphDetailsTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FDialogueGraphEditorApp> InApp);

private:
	TWeakPtr<FDialogueGraphEditorApp> DialogueGraphApp;
};
