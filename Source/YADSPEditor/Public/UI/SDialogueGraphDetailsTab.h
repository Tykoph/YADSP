// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphEditorApp.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

/**
 * Slate widget representing the details tab in the dialogue graph editor.
 */
class YADSPEDITOR_API SDialogueGraphDetailsTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphDetailsTab) {}
	SLATE_END_ARGS()
	
	/**
	 * Constructs the details tab widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InApp Pointer to the hosting dialogue graph editor application.
	 */
	void Construct(const FArguments& InArgs, TSharedPtr<FDialogueGraphEditorApp> InApp);
protected:
	FReply OnColorBlockClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	
private:
	TWeakPtr<FDialogueGraphEditorApp> DialogueGraphApp;
};
