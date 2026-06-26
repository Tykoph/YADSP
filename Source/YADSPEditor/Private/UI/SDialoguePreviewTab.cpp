// Copyright Tom Duby. All Rights Reserved.

#include "UI/SDialoguePreviewTab.h"
#include "GSheetLocSystemLibrary.h"
#include "DialogueSystem.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueGraphSettings.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Styling/SlateStyle.h"
#include "Styling/CoreStyle.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"

void SDialoguePreviewTab::Construct(const FArguments& InArgs, TSharedPtr<DialogueGraphEditorApp> InApp)
{
	DialogueGraphApp = InApp;
	UDialogueGraphSettings* Settings = UDialogueGraphSettings::Get();
	
	TArray<TSharedRef<ITextDecorator>> Decorators;

	URichTextBlock* DummyOwner = NewObject<URichTextBlock>(GetTransientPackage());
	DummyOwner->AddToRoot();
	InstantiatedObjects.Add(DummyOwner);
	DummyOwner->TakeWidget(); // Forces initialization of the internal StyleInstance

	for (TSubclassOf<URichTextBlockDecorator> DecoratorClass : Settings->PreviewDecorators) {
		if (DecoratorClass) {
			if (URichTextBlockDecorator* Decorator = NewObject<URichTextBlockDecorator>(GetTransientPackage(), DecoratorClass)) {
				Decorator->AddToRoot();
				InstantiatedObjects.Add(Decorator);
				
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
			.TextStyle(&Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->SpeakerPreviewStyle))
			.Decorators(Decorators)
			.AutoWrapText(this)
		]
		+ SScrollBox::Slot()
		.Padding(10.0f)
		[
			SAssignNew(DialogueRichTextBlock, SRichTextBlock)
			.Text(FText::GetEmpty())
			.DecoratorStyleSet(Settings->GetRichTextStyleSet().Get())
			.TextStyle(&Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->DialoguePreviewStyle))
			.Decorators(Decorators)
			.WrapTextAt(500.0f)
		]
	];

	if (InApp.IsValid()) {
		SelectionChangedHandle = InApp->OnGraphSelectionChangedDelegate.AddRaw(this, &SDialoguePreviewTab::OnGraphSelectionChanged);
	}
	
	StyleChangedHandle = Settings->OnRichTextStyleChanged.AddRaw(this, &SDialoguePreviewTab::OnRichTextStyleChanged);
}

SDialoguePreviewTab::~SDialoguePreviewTab()
{
	for (UObject* Object : InstantiatedObjects) {
		if (Object) {
			Object->RemoveFromRoot();
		}
	}
	InstantiatedObjects.Empty();

	if (const TSharedPtr<DialogueGraphEditorApp> App = DialogueGraphApp.Pin()) {
		App->OnGraphSelectionChangedDelegate.Remove(SelectionChangedHandle);
	}
	
	if (UDialogueGraphSettings* Settings = UDialogueGraphSettings::Get()) {
		Settings->OnRichTextStyleChanged.Remove(StyleChangedHandle);
	}
}

void SDialoguePreviewTab::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet)
{
	CachedSelection = SelectionSet;
	
	FText NewPreviewText = FText::GetEmpty();
	FText NewSpeakerPreviewText = FText::GetEmpty();

	if (CurrentNode) {
		CurrentNode->OnPropertiesChanged.Remove(PropertyChangedHandle);
	}
	
	for (UObject* Obj : SelectionSet) {
		if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(Obj)) {
			if (UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(TextNode->GetNodeInfo())) {
				const FString Language = UDialogueGraphSettings::Get()->GetPreviewLanguage();
				
				CurrentNode = NodeInfo;
				
				if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable && !NodeInfo->DialogueID.IsNone()) {
					FDataTableRowHandle Handle;
					Handle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
					Handle.RowName = NodeInfo->DialogueID;
					NewPreviewText = FText::FromString(UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language));

					FString CombinedSpeakers;
					for (const FName& ID : NodeInfo->SpeakerIDs) {
						if (ID.IsNone()) continue;
						if (!CombinedSpeakers.IsEmpty()) CombinedSpeakers += TEXT(", ");
				
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
				
				PropertyChangedHandle = NodeInfo->OnPropertiesChanged.AddRaw(this, &SDialoguePreviewTab::RefreshPreview);
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

void SDialoguePreviewTab::OnRichTextStyleChanged() const
{
	if (UDialogueGraphSettings* Settings = UDialogueGraphSettings::Get()) {
		if (SpeakerRichTextBlock.IsValid()) {
			SpeakerRichTextBlock->SetDecoratorStyleSet(Settings->GetRichTextStyleSet().Get());
			SpeakerRichTextBlock->SetTextStyle(Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->SpeakerPreviewStyle));
		}
		if (DialogueRichTextBlock.IsValid()) {
			DialogueRichTextBlock->SetDecoratorStyleSet(Settings->GetRichTextStyleSet().Get());
			DialogueRichTextBlock->SetTextStyle(Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->DialoguePreviewStyle));
		}
	}
}
