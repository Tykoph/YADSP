// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "SGraphNode.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueGraphNodeText.h"

/**
 * Slate widget for the visual representation of a text node in the dialogue graph.
 */
class SDialogueGraphNodeText : public SGraphNode
{
public:
	virtual ~SDialogueGraphNodeText() override {};
	SLATE_BEGIN_ARGS(SDialogueGraphNodeText) {}
	SLATE_END_ARGS()

	/**
	 * Constructs the text node widget.
	 * @param InArgs The Slate arguments for this widget.
	 * @param InNode Pointer to the corresponding dialogue graph node.
	 */
	void Construct(const FArguments& InArgs, UDialogueGraphNodeText* InNode);

	virtual void UpdateGraphNode() override;
	
protected:
	/**
	 * Helper to get the hosting editor app to access settings like Language.
	 * @return A shared pointer to the dialogue graph editor app.
	 */
	TSharedPtr<FDialogueGraphEditorApp> GetGraphEditorApp() const;
	
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> InMainBox) override;

	/**
	 * Retrieves the text to display for the speaker preview.
	 * @return The formatted speaker text.
	 */
	FText GetPreviewSpeakerText() const;
	
	/**
	 * Retrieves the text to display for the dialogue preview.
	 * @return The formatted dialogue text.
	 */
	FText GetPreviewDialogueText() const;
	
	/** Validates and updates the preview text caches for speaker and dialogue if they are out of date. */
	void EnsurePreviewCacheUpToDate() const; 
	
	/**
	 * Callback fired when a dialogue option is selected from the combo box.
	 * @param InNewSelection The newly selected string option.
	 * @param SelectInfo The method used to select the option.
	 */
	void OnDialogueSelected(TSharedPtr<FString> InNewSelection, ESelectInfo::Type SelectInfo);
	
	/**
	 * Retrieves the current text to display in the dialogue selection combo box.
	 * @return The selected dialogue text.
	 */
	FText GetDialogueComboText() const;
	
	/** Rebuilds the UI representation of the speaker list to reflect the current data state. */
	void RefreshSpeakerList();
	
	/** Callback fired when the user requests to add a new speaker. */
	void OnAddSpeaker();
	
	/**
	 * Callback fired when the user requests to remove an existing speaker.
	 * @param InIndex The index of the speaker to remove.
	 */
	void OnRemoveSpeaker(int32 InIndex);
	
	/**
	 * Callback fired when a speaker selection is changed in the combo box.
	 * @param InNewSelection The newly selected speaker string.
	 * @param SelectInfo The method used to select the option.
	 * @param InIndex The index of the speaker being modified.
	 */
	void OnSpeakerComboChanged(const TSharedPtr<FString>& InNewSelection, ESelectInfo::Type SelectInfo, int32 InIndex) const;
	
	/**
	 * Retrieves the current text to display in the speaker selection combo box for a given index.
	 * @param InIndex The index of the speaker.
	 * @return The selected speaker text.
	 */
	FText GetSpeakerComboText(int32 InIndex) const;

	/** Updates the cached text preview for the speaker label. */
	void UpdateSpeakerPreview() const;
	
	/** Updates the cached text preview for the dialogue content. */
	void UpdateDialoguePreview() const;
	
	// Multiple Speakers Support
	TSharedPtr<SVerticalBox> SpeakerListContainer;

private:
	TArray<TSharedPtr<FString>> SpeakerOptions;
	TArray<TSharedPtr<FString>> DialogueOptions;

	// Cache for preview text to avoid redundant lookups
	mutable FText CachedSpeakerPreview;
	mutable FText CachedDialoguePreview;
	mutable FString LastPreviewLanguage;
};
