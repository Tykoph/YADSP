// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueGraphEditorApp.h"
#include "Nodes/DialogueGraphNodeBranch.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/SRichTextBlock.h"

class YADSPEDITOR_API SDialoguePreviewTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialoguePreviewTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FDialogueGraphEditorApp> InApp);

	virtual ~SDialoguePreviewTab() override;

private:
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& InSelectionSet);
	void OnRichTextStyleChanged() const;
	void RefreshPreview();
	void ProcessTextNode(FText& NewPreviewText, FText& NewSpeakerPreviewText, const UDialogueGraphNodeText* TextNode);
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
	FDelegateHandle StyleChangedHandle;
	FDelegateHandle PropertyChangedHandle;
};
