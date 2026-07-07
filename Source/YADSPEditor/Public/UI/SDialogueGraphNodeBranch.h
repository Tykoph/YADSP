// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "SGraphNode.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueGraphNodeBranch.h"

/**
 * Slate widget for the visual representation of a branch node in the dialogue graph.
 */
class SDialogueGraphNodeBranch : public SGraphNode
{
public:
	virtual ~SDialogueGraphNodeBranch() override {};
	SLATE_BEGIN_ARGS(SDialogueGraphNodeBranch) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the branch node widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InNode Pointer to the corresponding dialogue graph node.
	 */
	void Construct(const FArguments& InArgs, UDialogueGraphNodeBranch* InNode);
	
	void UpdateGraphNode() override;
	
protected:
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> InMainBox) override;

	/**
	 * Helper to get the hosting editor app to access settings like Language.
	 * @return A shared pointer to the dialogue graph editor app.
	 */
	TSharedPtr<FDialogueGraphEditorApp> GetGraphEditorApp() const;

	/** Updates the cached text preview for dialogue options based on the current configuration. */
	void UpdateOptionTextPreview() const;
	
	/**
	 * Retrieves the preview text for a specific dialogue option.
	 * @param Index The index of the option.
	 * @return The formatted preview text.
	 */
	FText GetPreviewOptionText(int32 Index) const;
	
	/** Validates and updates the preview text cache if it is out of date. */
	void EnsurePreviewCacheUpToDate() const; 

private:
	TArray<TSharedPtr<FString>> DialogueOptions;

	// Cache for preview text to avoid redundant lookups
	mutable TArray<FText> CachedOptionsTextPreview;
	mutable FString LastPreviewLanguage;
};
