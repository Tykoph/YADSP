// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueGraphNodeText.h"

#include "DialogueGraphEditorApp.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"
#include "DialogueNodeInfoText.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SComboBox.h"
#include "Toolkits/ToolkitManager.h"
#include "GSheetLocSystemLibrary.h"
#include "ScopedTransaction.h"

FText UDialogueGraphNodeText::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(NodeInfoPtr);

	if (NodeInfo->Title.IsEmpty()) {
		FString DialogueTextStr = NodeInfo->DialogueID.ToString();
		if (DialogueTextStr.Len() > 15) {
			DialogueTextStr = DialogueTextStr.Left(15) + TEXT("...");
		}
		return FText::FromString(DialogueTextStr);
	}

	return FText::FromString(NodeInfo->Title);
}

void UDialogueGraphNodeText::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Text Node Actions")));

	UDialogueGraphNodeText* Node = const_cast<UDialogueGraphNodeText*>(this);
	Section.AddMenuEntry(
		"DeleteEntry",
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Delete this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeleteNodeIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node]()
			{
				Node->GetGraph()->RemoveNode(Node);
			}
		))
	);
}

UEdGraphPin* UDialogueGraphNodeText::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName Category = TEXT("Input");
	FName SubCategory = TEXT("TextPin");

	UEdGraphPin* Pin = CreatePin(
		Dir,
		Category,
		Name
	);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}

UEdGraphPin* UDialogueGraphNodeText::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Input")));
}

void UDialogueGraphNodeText::CreateDefaultOutputPin()
{
	FString DefaultResponse = TEXT("Continue");
	CreateDialoguePin(EGPD_Output, FName(DefaultResponse));
	GetDialogueNodeInfo()->DialogueResponses.Add(FText::FromString(DefaultResponse));
}

void UDialogueGraphNodeText::SyncWithNodeResponse()
{
	UDialogueNodeInfoText* NodeInfo = GetDialogueNodeInfo();
	int NumGraphNodePins = Pins.Num() - 1;
	int NumInfoPins = NodeInfo->DialogueResponses.Num();

	while (NumGraphNodePins > NumInfoPins) {
		RemovePinAt(NumGraphNodePins - 1, EGPD_Output);
		NumGraphNodePins--;
	}

	while (NumInfoPins > NumGraphNodePins) {
		CreateDialoguePin(
			EGPD_Output,
			FName(NodeInfo->DialogueResponses[NumGraphNodePins].ToString())
		);
		NumGraphNodePins++;
	}

	int Index = 1;
	for (const FText& Response : NodeInfo->DialogueResponses) {
		GetPinAt(Index)->PinName = FName(Response.ToString());
		Index++;
	}
}

//
// SDialogueGraphNodeText
//

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
	if (UEdGraph* Graph = GraphNode->GetGraph()) {
		if (UDialogueSystem* Asset = Cast<UDialogueSystem>(Graph->GetOuter())) {
			TSharedPtr<IToolkit> Toolkit = FToolkitManager::Get().FindEditorForAsset(Asset);
			return StaticCastSharedPtr<DialogueGraphEditorApp>(Toolkit);
		}
	}
	return nullptr;
}

void SDialogueGraphNodeText::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
	UDialogueNodeInfoText* NodeInfo = TextNode ? TextNode->GetDialogueNodeInfo() : nullptr;

	if (!NodeInfo) {
		return;
	}
	
	// Initialize Options
	SpeakerOptions.Empty();
	TArray<FString> Speakers = NodeInfo->GetSpeakerOptions();
	for (const FString& S : Speakers) {
		SpeakerOptions.Add(MakeShared<FString>(S));
	}
	
	DialogueOptions.Empty();
	TArray<FString> Dialogues = NodeInfo->GetDialogueOptions();
	for (const FString& D : Dialogues) {
		DialogueOptions.Add(MakeShared<FString>(D));
	}
	
	TSharedPtr<FString> CurrentDialogue = nullptr;
	FString CurrentDialogueID = NodeInfo->DialogueID.ToString();
	for (const auto& Ptr : DialogueOptions) {
		if (*Ptr == CurrentDialogueID) {
			CurrentDialogue = Ptr;
			break;
		}
	}

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
			.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
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
	UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode);
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
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
				{
					return SNew(STextBlock).Text(FText::FromString(*Item));
				})
				.OnSelectionChanged_Lambda([this, i](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
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
	if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
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
	if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
			return FText::FromName(NodeInfo->DialogueID);
		}
	}

	return FText::GetEmpty();
}

void SDialogueGraphNodeText::UpdateSpeakerPreview()
{
	if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
			FString Language = TEXT("en-US");

			if (TSharedPtr<DialogueGraphEditorApp> App = GetGraphEditorApp()) {
				Language = App->GetPreviewLanguage();
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
			CachedSpeakerPreview = FText::FromString(CombinedSpeakers);
		}
	}
	else {
		CachedSpeakerPreview = FText::GetEmpty();
	}
}

void SDialogueGraphNodeText::UpdateDialoguePreview()
{
	if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(GraphNode)) {
		if (UDialogueNodeInfoText* NodeInfo = TextNode->GetDialogueNodeInfo()) {
			// Get Language from Editor App

			FString Language = TEXT("en-US");

			if (TSharedPtr<DialogueGraphEditorApp> App = GetGraphEditorApp()) {
				Language = App->GetPreviewLanguage();
			}

			if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable && !NodeInfo->DialogueID.IsNone()) {
				FDataTableRowHandle Handle;
				Handle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
				Handle.RowName = NodeInfo->DialogueID;
				CachedDialoguePreview = FText::FromString(UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language));
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
	if (TSharedPtr<DialogueGraphEditorApp> App = GetGraphEditorApp()) {
		CurrentLanguage = App->GetPreviewLanguage();
	}

	if (CurrentLanguage != LastPreviewLanguage) {
		LastPreviewLanguage = CurrentLanguage;
		const_cast<SDialogueGraphNodeText*>(this)->UpdateSpeakerPreview();
		const_cast<SDialogueGraphNodeText*>(this)->UpdateDialoguePreview();
	}

	return CachedSpeakerPreview;
}

FText SDialogueGraphNodeText::GetPreviewDialogueText() const
{
	// Language check is done in GetPreviewSpeakerText typically, but to be safe:
	FString CurrentLanguage = TEXT("en-US");
	if (TSharedPtr<DialogueGraphEditorApp> App = GetGraphEditorApp()) {
		CurrentLanguage = App->GetPreviewLanguage();
	}

	if (CurrentLanguage != LastPreviewLanguage) {
		LastPreviewLanguage = CurrentLanguage;
		const_cast<SDialogueGraphNodeText*>(this)->UpdateSpeakerPreview();
		const_cast<SDialogueGraphNodeText*>(this)->UpdateDialoguePreview();
	}

	return CachedDialoguePreview;
}
