// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "SGraphNode.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueGraphNodeText.h"

class SDialogueGraphNodeText : public SGraphNode
{
public:
	virtual ~SDialogueGraphNodeText() override;
	SLATE_BEGIN_ARGS(SDialogueGraphNodeText)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UDialogueGraphNodeText* InNode);

	virtual void UpdateGraphNode() override;
	
protected:
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> InMainBox) override;

	FText GetPreviewSpeakerText() const;
	FText GetPreviewDialogueText() const;
	void EnsurePreviewCacheUpToDate() const; 
	
	// Helper to get the hosting editor app to access settings like Language
	TSharedPtr<FDialogueGraphEditorApp> GetGraphEditorApp() const;

	TSharedPtr<FString> CurrentSpeakerSelection;
	TSharedPtr<FString> CurrentDialogueSelection;

	void OnDialogueSelected(TSharedPtr<FString> InNewSelection, ESelectInfo::Type SelectInfo);

	FText GetDialogueComboText() const;

	// Multiple Speakers Support
	TSharedPtr<SVerticalBox> SpeakerListContainer;
	void RefreshSpeakerList();
	void OnAddSpeaker();
	void OnRemoveSpeaker(int32 InIndex);
	void OnSpeakerComboChanged(const TSharedPtr<FString>& InNewSelection, ESelectInfo::Type SelectInfo, int32 InIndex) const;
	FText GetSpeakerComboText(int32 InIndex) const;

	void UpdateSpeakerPreview() const;
	void UpdateDialoguePreview() const;

private:
	TArray<TSharedPtr<FString>> SpeakerOptions;
	TArray<TSharedPtr<FString>> DialogueOptions;

	// Cache for preview text to avoid redundant lookups
	mutable FText CachedSpeakerPreview;
	mutable FText CachedDialoguePreview;
	mutable FString LastPreviewLanguage;
};
