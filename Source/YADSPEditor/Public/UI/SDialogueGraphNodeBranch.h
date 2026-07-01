// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "SGraphNode.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueGraphNodeBranch.h"

class SDialogueGraphNodeBranch : public SGraphNode
{
public:
	virtual ~SDialogueGraphNodeBranch() override {};
	SLATE_BEGIN_ARGS(SDialogueGraphNodeBranch)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UDialogueGraphNodeBranch* InNode);
	
	void UpdateGraphNode() override;
	
protected:
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> InMainBox) override;

	// Helper to get the hosting editor app to access settings like Language
	TSharedPtr<FDialogueGraphEditorApp> GetGraphEditorApp() const;

	void UpdateOptionTextPreview() const;
	FText GetPreviewOptionText(int32 Index) const;
	
	void EnsurePreviewCacheUpToDate() const; 

private:
	TArray<TSharedPtr<FString>> DialogueOptions;

	// Cache for preview text to avoid redundant lookups
	mutable TArray<FText> CachedOptionsTextPreview;
	mutable FString LastPreviewLanguage;
};
