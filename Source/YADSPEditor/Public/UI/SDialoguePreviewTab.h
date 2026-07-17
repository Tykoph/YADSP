// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueGraphNodeBranch.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/SRichTextBlock.h"

/**
 * Slate widget for the preview tab, providing a runtime-like view of dialogue nodes.
 */
class YADSPEDITOR_API SDialoguePreviewTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialoguePreviewTab) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the preview tab widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InApp Pointer to the hosting dialogue graph editor application.
	 */
	void Construct(const FArguments& InArgs, TSharedPtr<FDialogueGraphEditorApp> InApp);

	virtual ~SDialoguePreviewTab() override;

private:
	/**
	 * Callback fired when the selection in the graph panel changes.
	 * @param InSelectionSet The new set of selected graph nodes.
	 */
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& InSelectionSet);
	
	/** Callback fired when the active rich text style is modified. */
	void OnRichTextStyleChanged() const;
	
	/** Updates the preview UI to reflect the currently selected dialogue node. */
	void RefreshPreview();
	
	/**
	 * Processes a text node to extract its preview strings.
	 * @param NewPreviewText Extracted dialogue text.
	 * @param NewSpeakerPreviewText Extracted speaker text.
	 * @param TextNode The text node being processed.
	 */
	void ProcessTextNode(FText& NewPreviewText, FText& NewSpeakerPreviewText, const UDialogueGraphNodeText* TextNode);
	
	/**
	 * Processes a branch node to configure the options preview.
	 * @param BranchNode The branch node being processed.
	 */
	void ProcessBranchNode(const UDialogueGraphNodeBranch* BranchNode);


	TWeakObjectPtr<UDialogueNodeInfoBase> CurrentNode;
	
	TWeakPtr<FDialogueGraphEditorApp> DialogueGraphApp;
	
	TSharedPtr<SRichTextBlock> DialogueRichTextBlock;
	TSharedPtr<SRichTextBlock> SpeakerRichTextBlock;
	TSharedPtr<SBorder> SpeakerBox;
	TSharedPtr<SBorder> DialogueBox;
	TSharedPtr<SVerticalBox> OptionsBox;
	
	TArray<FString> NewOptionsPreviewTexts;

	TArray<TSharedRef<ITextDecorator>> CachedDecorators;
	TArray<TObjectPtr<UObject>> InstantiatedObjects;
	FGraphPanelSelectionSet CachedSelection;
	
	FDelegateHandle SelectionChangedHandle;
	FDelegateHandle LanguageChangedHandle;
	FDelegateHandle StyleChangedHandle;
	FDelegateHandle PropertyChangedHandle;
};
