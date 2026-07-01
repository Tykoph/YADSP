// Copyright Tom Duby. All Rights Reserved.

#include "UI/SDialogueGraphNodeBranch.h"

#include "DialogueGraphProjectSettings.h"
#include "DialogueGraphUserSettings.h"
#include "GSheetLocSystemLibrary.h"
#include "YADSP.h"
#include "Toolkits/ToolkitManager.h"

void SDialogueGraphNodeBranch::Construct(const FArguments& InArgs, UDialogueGraphNodeBranch* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SDialogueGraphNodeBranch::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
	UpdateOptionTextPreview();
}

TSharedPtr<FDialogueGraphEditorApp> SDialogueGraphNodeBranch::GetGraphEditorApp() const
{
	if (const UEdGraph* Graph = GraphNode->GetGraph()) {
		if (const UDialogueSystem* Asset = Cast<UDialogueSystem>(Graph->GetOuter())) {
			const TSharedPtr<IToolkit> Toolkit = FToolkitManager::Get().FindEditorForAsset(Asset);
			return StaticCastSharedPtr<FDialogueGraphEditorApp>(Toolkit);
		}
	}
	return nullptr;
}

void SDialogueGraphNodeBranch::CreateBelowPinControls(const TSharedPtr<SVerticalBox> InMainBox)
{
	const UDialogueGraphNodeBranch* BranchNode = Cast<UDialogueGraphNodeBranch>(GraphNode);
	const UDialogueNodeInfoBranch* NodeInfo = BranchNode ? Cast<UDialogueNodeInfoBranch>(BranchNode->GetNodeInfo()) : nullptr;

	if (NodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("SDialogueGraphNodeBranch::CreateBelowPinControls -> NodeInfo is nullptr"))
		return;
	}

	const TSharedPtr<SVerticalBox> OptionsBox = SNew(SVerticalBox);
	
	// Preview Separator
	OptionsBox->AddSlot()
	.AutoHeight()
	.Padding(0.0f, 10.0f)
	[
		SNew(SBox)
		.HeightOverride(1.0f)
		[
			SNew(SImage)
			.ColorAndOpacity(FLinearColor::Gray)
		]
	];

	for (int32 i = 0; i < NodeInfo->BranchOptions.Num(); ++i)
	{
		OptionsBox->AddSlot()
		.AutoHeight()
		.Padding(0.0f, 2.0f, 0.0f, 2.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			.BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.05f, 0.5f))
			.Padding(FMargin(5.0f))
			[
				SNew(STextBlock)
				.Text(this, &SDialogueGraphNodeBranch::GetPreviewOptionText, i)
				.AutoWrapText(true)
				.WrapTextAt(200.0f)
			]
		];
	}
	
	InMainBox->AddSlot().AutoHeight().Padding(10.0f, 5.0f)
	[
		OptionsBox.ToSharedRef()
	];
}

void SDialogueGraphNodeBranch::UpdateOptionTextPreview() const
{
	CachedOptionsTextPreview.Empty();

	if (const UDialogueGraphNodeBranch* TextNode = Cast<UDialogueGraphNodeBranch>(GraphNode)) {
		if (UDialogueNodeInfoBranch* NodeInfo = Cast<UDialogueNodeInfoBranch>(TextNode->GetNodeInfo())) {
			FString Language = TEXT("en-US");

			if (TSharedPtr<FDialogueGraphEditorApp> App = GetGraphEditorApp()) {
				Language = UDialogueGraphProjectSettings::Get()->GetPreviewLanguage();
			}

			for (const FBranchCondition& ID : NodeInfo->BranchOptions) {
				if (ID.DialogueResponseKey.IsNone()) {
					CachedOptionsTextPreview.Add(FText::GetEmpty());
					continue;
				}
				
				FString OptionText;
				if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable) {
					FDataTableRowHandle Handle;
					Handle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
					Handle.RowName = ID.DialogueResponseKey;
					OptionText = UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language);
				}
				else {
					OptionText = ID.DialogueResponseKey.ToString();
				}
				
				FText FinalText = FText::FromString(UDialogueGraphUserSettings::Get()->bDisplayRichTextFlags ? 
					OptionText : UGSheetLocSystemLibrary::StripRichTextTags(OptionText));
				CachedOptionsTextPreview.Add(FinalText);
			}
		}
	}
}

FText SDialogueGraphNodeBranch::GetPreviewOptionText(int32 Index) const
{
	EnsurePreviewCacheUpToDate();
	if (CachedOptionsTextPreview.IsValidIndex(Index))
	{
		return CachedOptionsTextPreview[Index];
	}
	return FText::GetEmpty();
}

void SDialogueGraphNodeBranch::EnsurePreviewCacheUpToDate() const
{
	FString CurrentLanguage = UDialogueGraphProjectSettings::Get()->GetPreviewLanguage();
	
	if (CurrentLanguage != LastPreviewLanguage) {
		LastPreviewLanguage = CurrentLanguage;
		UpdateOptionTextPreview();
	}
}
