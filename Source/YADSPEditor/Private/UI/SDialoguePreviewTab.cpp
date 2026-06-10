// Copyright 2026 Tom Duby. All Rights Reserved.

#include "UI/SDialoguePreviewTab.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueGraphSettings.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Styling/SlateStyle.h"
#include "Styling/CoreStyle.h"
#include "GSheetLocSystemLibrary.h"
#include "DialogueSystem.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"

void SDialoguePreviewTab::Construct(const FArguments& InArgs, TSharedPtr<DialogueGraphEditorApp> InApp)
{
	DialogueGraphApp = InApp;
	UDialogueGraphSettings* Settings = UDialogueGraphSettings::Get();
	
	TArray<TSharedRef<ITextDecorator>> Decorators;

	URichTextBlock* DummyOwner = GetMutableDefault<URichTextBlock>();
	for (TSubclassOf<URichTextBlockDecorator> DecoratorClass : Settings->PreviewDecorators) {
		if (DecoratorClass) {
			if (URichTextBlockDecorator* Decorator = NewObject<URichTextBlockDecorator>(GetTransientPackage(), DecoratorClass)) {
				Decorator->AddToRoot();
				InstantiatedDecorators.Add(Decorator);
				
				TSharedPtr<ITextDecorator> CreatedDecorator = Decorator->CreateDecorator(DummyOwner);
				if (CreatedDecorator.IsValid()) {
					Decorators.Add(CreatedDecorator.ToSharedRef());
				}
			}
		}
	}

	Decorators.Add(SRichTextBlock::ImageDecorator());

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		.Padding(10.0f)
		[
			SAssignNew(SpeakerRichTextBlock, SRichTextBlock)
			.Text(FText::GetEmpty())
			.DecoratorStyleSet(Settings->GetRichTextStyleSet().Get())
			.Decorators(Decorators)
			.AutoWrapText(this)
		]
		+ SScrollBox::Slot()
		.Padding(10.0f)
		[
			SAssignNew(DialogueRichTextBlock, SRichTextBlock)
			.Text(FText::GetEmpty())
			.DecoratorStyleSet(Settings->GetRichTextStyleSet().Get())
			.Decorators(Decorators)
			.WrapTextAt(500.0f)
		]
	];

	if (InApp.IsValid()) {
		SelectionChangedHandle = InApp->OnGraphSelectionChangedDelegate.AddRaw(this, &SDialoguePreviewTab::OnGraphSelectionChanged);
	}
}

SDialoguePreviewTab::~SDialoguePreviewTab()
{
	for (URichTextBlockDecorator* Decorator : InstantiatedDecorators)
	{
		if (Decorator) {
			Decorator->RemoveFromRoot();
		}
	}
	InstantiatedDecorators.Empty();

	if (const TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin()) {
		App->OnGraphSelectionChangedDelegate.Remove(SelectionChangedHandle);
	}
}

void SDialoguePreviewTab::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet)
{
	CachedSelection = SelectionSet;
	
	FText NewPreviewText = FText::GetEmpty();
	FText NewSpeakerPreviewText = FText::GetEmpty();

	for (UObject* Obj : SelectionSet) {
		if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(Obj)) {
			if (const UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(TextNode->GetNodeInfo())) {
				const FString Language = UDialogueGraphSettings::Get()->GetPreviewLanguage();
				if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable && !NodeInfo->DialogueID.IsNone()) {
					FDataTableRowHandle Handle;
					Handle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
					Handle.RowName = NodeInfo->DialogueID;
					NewPreviewText = FText::FromString(UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language));

					FString CombinedSpeakers;
					for (const FName& ID : NodeInfo->SpeakerIDs) {
						if (ID.IsNone()) continue;
						if (!CombinedSpeakers.IsEmpty()) CombinedSpeakers += TEXT("<Speaker>, </>");
				
						if (NodeInfo->DialogueSystem->SpeakerDataTable) {
							FDataTableRowHandle SpeakerHandle;
							SpeakerHandle.DataTable = NodeInfo->DialogueSystem->SpeakerDataTable;
							SpeakerHandle.RowName = ID;
							CombinedSpeakers += UGSheetLocSystemLibrary::GetLocalizedStringManual(SpeakerHandle, Language);
						}
						else {
							CombinedSpeakers += ID.ToString();
						}
					}
					NewSpeakerPreviewText = FText::FromString(CombinedSpeakers);
				}
				else if (!NodeInfo->DialogueID.IsNone()) {
					NewPreviewText = FText::FromString(NodeInfo->DialogueID.ToString());
				}
				break; // Use only the first selected node
			}
		}
	}

	if (DialogueRichTextBlock.IsValid()) {
		DialogueRichTextBlock->SetText(NewPreviewText);
	}
	if (SpeakerRichTextBlock.IsValid()) {
		SpeakerRichTextBlock->SetText(NewSpeakerPreviewText);
	}
}

void SDialoguePreviewTab::RefreshPreview()
{
	if (CachedSelection.IsEmpty()) return;
	OnGraphSelectionChanged(CachedSelection);
}
