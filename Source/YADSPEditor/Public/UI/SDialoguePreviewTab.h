// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/SRichTextBlock.h"

class YADSPEDITOR_API SDialoguePreviewTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialoguePreviewTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<DialogueGraphEditorApp> InApp);

	virtual ~SDialoguePreviewTab() override;

private:
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet);
	void OnRichTextStyleChanged() const;
	void RefreshPreview();

	UDialogueNodeInfoText* CurrentNode = nullptr;
	
	TWeakPtr<DialogueGraphEditorApp> DialogueGraphApp;
	
	TSharedPtr<SRichTextBlock> DialogueRichTextBlock;
	TSharedPtr<SRichTextBlock> SpeakerRichTextBlock;
	
	TArray<UObject*> InstantiatedObjects;
	FGraphPanelSelectionSet CachedSelection;
	
	FDelegateHandle SelectionChangedHandle;
	FDelegateHandle StyleChangedHandle;
	FDelegateHandle PropertyChangedHandle;
};
