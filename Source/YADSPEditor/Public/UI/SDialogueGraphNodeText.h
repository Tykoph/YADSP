// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "SGraphNode.h"
#include "YADSPEditor/Public/DialogueGraphEditorApp.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeText.h"

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
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;

	FText GetPreviewSpeakerText() const;
	FText GetPreviewDialogueText() const;

	// Helper to get the hosting editor app to access settings like Language
	TSharedPtr<DialogueGraphEditorApp> GetGraphEditorApp() const;

	TSharedPtr<FString> CurrentSpeakerSelection;
	TSharedPtr<FString> CurrentDialogueSelection;

	void OnDialogueSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	FText GetDialogueComboText() const;

	// Multiple Speakers Support
	TSharedPtr<SVerticalBox> SpeakerListContainer;
	void RefreshSpeakerList();
	void OnAddSpeaker();
	void OnRemoveSpeaker(int32 Index);
	void OnSpeakerComboChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo, int32 Index);
	FText GetSpeakerComboText(int32 Index) const;

	void UpdateSpeakerPreview();
	void UpdateDialoguePreview();

private:
	TArray<TSharedPtr<FString>> SpeakerOptions;
	TArray<TSharedPtr<FString>> DialogueOptions;

	// Cache for preview text to avoid redundant lookups
	FText CachedSpeakerPreview;
	FText CachedDialoguePreview;
	mutable FString LastPreviewLanguage;
};
