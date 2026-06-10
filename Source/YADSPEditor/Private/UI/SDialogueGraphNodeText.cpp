// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/UI/SDialogueGraphNodeText.h"

#include "DialogueGraphSettings.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeText.h"

#include "DialogueSystem.h"
#include "GSheetLocSystemLibrary.h"
#include "Toolkits/ToolkitManager.h"
#include "Widgets/Text/SRichTextBlock.h"

SDialogueGraphNodeText::~SDialogueGraphNodeText()
{
}

void SDialogueGraphNodeText::Construct(const FArguments& InArgs, UDialogueGraphNodeText* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

void SDialogueGraphNodeText::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
	UpdateSpeakerPreview();
	UpdateDialoguePreview();
}

TSharedPtr<DialogueGraphEditorApp> SDialogueGraphNodeText::GetGraphEditorApp() const
{
	if (const UEdGraph* Graph = GraphNode->GetGraph()) {
		if (const UDialogueSystem* Asset = Cast<UDialogueSystem>(Graph->GetOuter())) {
			const TSharedPtr<IToolkit> Toolkit = FToolkitManager::Get().FindEditorForAsset(Asset);
			return StaticCastSharedPtr<DialogueGraphEditorApp>(Toolkit);
		}
	}
	return nullptr;
}

void SDialogueGraphNodeText::CreateBelowPinControls(const TSharedPtr<SVerticalBox> MainBox)
{
	const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
	const UDialogueNodeInfoText* NodeInfo = TextNode ? TextNode->GetDialogueNodeInfo() : nullptr;

	if (!NodeInfo) {
		return;
	}
	
	// Initialize Options
	SpeakerOptions.Empty();
	TArray<FString> Speakers = NodeInfo->GetSpeakerFromTable();
	for (const FString& S : Speakers) {
		SpeakerOptions.Add(MakeShared<FString>(S));
	}
	
	DialogueOptions.Empty();
	TArray<FString> Dialogues = NodeInfo->GetDialogueFromTable();
	for (const FString& D : Dialogues) {
		DialogueOptions.Add(MakeShared<FString>(D));
	}
	
	TSharedPtr<FString> CurrentDialogue = nullptr;
	const FString CurrentDialogueID = NodeInfo->DialogueID.ToString();
	for (const auto& Ptr : DialogueOptions) {
		if (*Ptr == CurrentDialogueID) {
			CurrentDialogue = Ptr;
			break;
		}
	}
	
	TArray<TSharedRef<ITextDecorator>> TextDecorators;
	TextDecorators.Add(SRichTextBlock::ImageDecorator());
	
	// Prepare Speaker List Container
	SAssignNew(SpeakerListContainer, SVerticalBox);
	RefreshSpeakerList();
	
	MainBox->AddSlot().AutoHeight().Padding(10.0f, 5.0f)
	[
		SNew(SVerticalBox)

		// Speaker Label
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Speakers:"))
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
			.ColorAndOpacity(FLinearColor::Gray)
		]
		
		// Speaker List
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SpeakerListContainer.ToSharedRef()
		]

		// Add Speaker Button
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 5.0f)
		[
			SNew(SButton)
			.OnClicked_Lambda([this]()
			{
				OnAddSpeaker();
				return FReply::Handled();
			})
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString("+ Add Speaker"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
				.Justification(ETextJustify::Center)
			]
		]

		// Dialogue ID Label
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 5.0f, 0.0f, 0.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("Dialogue ID:"))
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
			.ColorAndOpacity(FLinearColor::Gray)
		]

		// Dialogue ComboBox
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&DialogueOptions)
			.OnGenerateWidget_Lambda([](const TSharedPtr<FString>& Item)
			{
				return SNew(STextBlock).Text(FText::FromString(*Item));
			})
			.OnSelectionChanged(this, &SDialogueGraphNodeText::OnDialogueSelected)
			.InitiallySelectedItem(CurrentDialogue)
			[
				SNew(STextBlock)
				.Text(this, &SDialogueGraphNodeText::GetDialogueComboText)
			]
		]
		
		// Preview Separator
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(0.0f, 5.0f)
        [
        	SNew(SBox)
        	.HeightOverride(1.0f)
        	[
        		SNew(SImage)
        		.ColorAndOpacity(FLinearColor::Gray)
        	]
        ]

        // Preview Text
        + SVerticalBox::Slot()
        .AutoHeight()
        [
        	SNew(STextBlock)
        	.Text(this, &SDialogueGraphNodeText::GetPreviewSpeakerText)
        	.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
        	.ColorAndOpacity(FLinearColor::Yellow)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(0.0f, 2.0f, 0.0f, 0.0f)
        [
        	SNew(STextBlock)
        	.Text(this, &SDialogueGraphNodeText::GetPreviewDialogueText)
        	.AutoWrapText(true)
        	.WrapTextAt(200.0f)
        ]
	];
}

void SDialogueGraphNodeText::RefreshSpeakerList()
{
	SpeakerListContainer->ClearChildren();
	
	const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
	UDialogueNodeInfoText* NodeInfo = TextNode ? TextNode->GetDialogueNodeInfo() : nullptr;
	
	if (!NodeInfo) return;
	
	for (int32 i = 0; i < NodeInfo->SpeakerIDs.Num(); ++i) {
		FString CurrentSpeakerID = NodeInfo->SpeakerIDs[i].ToString();
		TSharedPtr<FString> CurrentSpeakerItem = nullptr;

		for (const auto& Ptr : SpeakerOptions) {
			if (*Ptr == CurrentSpeakerID) {
				CurrentSpeakerItem = Ptr;
				break;
			}
		}
		
		SpeakerListContainer->AddSlot().AutoHeight().Padding(0.0f, 2.0f)
		[
			SNew(SHorizontalBox)

			// Speakers Combo box
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&SpeakerOptions)
				.OnGenerateWidget_Lambda([](const TSharedPtr<FString>& Item)
				{
					return SNew(STextBlock).Text(FText::FromString(*Item));
				})
				.OnSelectionChanged_Lambda([this, i](const TSharedPtr<FString>& NewSelection, const ESelectInfo::Type SelectInfo)
				{
					OnSpeakerComboChanged(NewSelection, SelectInfo, i);
				})
				.InitiallySelectedItem(CurrentSpeakerItem)
				[
					SNew(STextBlock)
					.Text_Lambda([this, i]() { return GetSpeakerComboText(i); })
				]
			]

			// Remove Speaker Button
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("X"))
				.OnClicked_Lambda([this, i]()
				{
					OnRemoveSpeaker(i);
					return FReply::Handled();
				})
				.ContentPadding(FMargin(5.0f, 0.0f))
			]
		];
	}
}


void SDialogueGraphNodeText::OnAddSpeaker()
{
	UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
	UDialogueNodeInfoText* NodeInfo = TextNode ? TextNode->GetDialogueNodeInfo() : nullptr;

	if (NodeInfo) {
		const FScopedTransaction Transaction(FText::FromString("Add Speaker"));
		NodeInfo->Modify();
		FName DefaultSpeaker = NAME_None;

		if (SpeakerOptions.Num() > 0 && SpeakerOptions[0].IsValid()) {
			DefaultSpeaker = FName(*SpeakerOptions[0]);
		}
		
		NodeInfo->SpeakerIDs.Add(DefaultSpeaker);
		TextNode->OnPropertiesChanged();
		RefreshSpeakerList();
		UpdateSpeakerPreview();
	}
}


void SDialogueGraphNodeText::OnRemoveSpeaker(int32 Index)
{
	UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
	UDialogueNodeInfoText* NodeInfo = TextNode ? TextNode->GetDialogueNodeInfo() : nullptr;

	if (NodeInfo && NodeInfo->SpeakerIDs.IsValidIndex(Index)) {
		const FScopedTransaction Transaction(FText::FromString("Remove Speaker"));
		NodeInfo->Modify();
		NodeInfo->SpeakerIDs.RemoveAt(Index);
		TextNode->OnPropertiesChanged();
		RefreshSpeakerList();
		UpdateSpeakerPreview();
	}
}


void SDialogueGraphNodeText::OnSpeakerComboChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo, int32 Index)
{
	if (NewSelection.IsValid()) {
		UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
		UDialogueNodeInfoText* NodeInfo = TextNode ? TextNode->GetDialogueNodeInfo() : nullptr;
		
		if (NodeInfo && NodeInfo->SpeakerIDs.IsValidIndex(Index)) {
			if (NodeInfo->SpeakerIDs[Index].ToString() != *NewSelection) {
				const FScopedTransaction Transaction(FText::FromString("Change Speaker"));
				NodeInfo->Modify();
				NodeInfo->SpeakerIDs[Index] = FName(**NewSelection);
				TextNode->OnPropertiesChanged();
				UpdateSpeakerPreview();
			}
		}
	}
}

void SDialogueGraphNodeText::OnDialogueSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid()) {
		if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
			if (UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
				if (NodeInfo->DialogueID.ToString() != *NewSelection) {
					const FScopedTransaction Transaction(FText::FromString("Change Dialogue ID"));
					NodeInfo->Modify();
					NodeInfo->DialogueID = FName(**NewSelection);
					TextNode->OnPropertiesChanged();
					UpdateDialoguePreview();
				}
			}
		}
	}
}

FText SDialogueGraphNodeText::GetSpeakerComboText(int32 Index) const
{
	if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
			if (NodeInfo->SpeakerIDs.IsValidIndex(Index)) {
				return FText::FromName(NodeInfo->SpeakerIDs[Index]);
			}
		}
	}
	return FText::GetEmpty();
}

FText SDialogueGraphNodeText::GetDialogueComboText() const
{
	if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (const UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
			return FText::FromName(NodeInfo->DialogueID);
		}
	}

	return FText::GetEmpty();
}

static FString StripRichTextTags(const FString& Input)
{
	FString Result = Input;
	while (true) {
		const int32 OpenIndex = Result.Find(TEXT("<"));
		if (OpenIndex == INDEX_NONE) break;

		const int32 CloseIndex = Result.Find(TEXT(">"), ESearchCase::IgnoreCase, ESearchDir::FromStart, OpenIndex);
		if (CloseIndex == INDEX_NONE) break;
		
		Result.RemoveAt(OpenIndex, CloseIndex - OpenIndex + 1);
	}
	return Result;
}

void SDialogueGraphNodeText::UpdateSpeakerPreview()
{
	if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
			FString Language = TEXT("en-US");

			if (TSharedPtr<DialogueGraphEditorApp> App = GetGraphEditorApp()) {
				Language = UDialogueGraphSettings::Get()->GetPreviewLanguage();
			}
			
			FString CombinedSpeakers;

			for (const FName& ID : NodeInfo->SpeakerIDs) {
				if (ID.IsNone()) continue;
				if (!CombinedSpeakers.IsEmpty()) CombinedSpeakers += TEXT(", ");
				
				if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->SpeakerDataTable) {
					FDataTableRowHandle Handle;
					Handle.DataTable = NodeInfo->DialogueSystem->SpeakerDataTable;
					Handle.RowName = ID;
					CombinedSpeakers += UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language);
				}
				else {
					CombinedSpeakers += ID.ToString();
				}
			}
			CachedSpeakerPreview = FText::FromString(StripRichTextTags(CombinedSpeakers));
		}
	}
	else {
		CachedSpeakerPreview = FText::GetEmpty();
	}
}

void SDialogueGraphNodeText::UpdateDialoguePreview()
{
	if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (const UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {

			FString Language = TEXT("en-US");

			if (TSharedPtr<DialogueGraphEditorApp> App = GetGraphEditorApp()) {
				Language = UDialogueGraphSettings::Get()->GetPreviewLanguage();
			}

			if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable && !NodeInfo->DialogueID.IsNone()) {
				FDataTableRowHandle Handle;
				Handle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
				Handle.RowName = NodeInfo->DialogueID;
				const FString LocalizedStr = UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language);
				CachedDialoguePreview = FText::FromString(StripRichTextTags(LocalizedStr));
			}
			else if (!NodeInfo->DialogueID.IsNone()) {
				CachedDialoguePreview = FText::FromString(NodeInfo->DialogueID.ToString());
			}
			else {
				CachedDialoguePreview = FText::GetEmpty();
			}
		}
	}
	else {
		CachedDialoguePreview = FText::GetEmpty();
	}
}

FText SDialogueGraphNodeText::GetPreviewSpeakerText() const
{
	FString CurrentLanguage = TEXT("en-US");
	CurrentLanguage = UDialogueGraphSettings::Get()->GetPreviewLanguage();

	if (CurrentLanguage != LastPreviewLanguage) {
		LastPreviewLanguage = CurrentLanguage;
		const_cast<SDialogueGraphNodeText*>(this)->UpdateSpeakerPreview();
		const_cast<SDialogueGraphNodeText*>(this)->UpdateDialoguePreview();
	}

	return CachedSpeakerPreview;
}

FText SDialogueGraphNodeText::GetPreviewDialogueText() const
{
	FString CurrentLanguage = TEXT("en-US");
	CurrentLanguage = UDialogueGraphSettings::Get()->GetPreviewLanguage();
	
	if (CurrentLanguage != LastPreviewLanguage) {
		LastPreviewLanguage = CurrentLanguage;
		const_cast<SDialogueGraphNodeText*>(this)->UpdateSpeakerPreview();
		const_cast<SDialogueGraphNodeText*>(this)->UpdateDialoguePreview();
	}

	return CachedDialoguePreview;
}
