// Copyright Tom Duby. All Rights Reserved.

#include "UI/SDialoguePreviewTab.h"
#include "GSheetLocSystemLibrary.h"
#include "DialogueSystem.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueGraphProjectSettings.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Styling/SlateStyle.h"
#include "Styling/CoreStyle.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"
#include "Nodes/DialogueGraphNodeBranch.h"

void SDialoguePreviewTab::Construct(const FArguments& InArgs, TSharedPtr<FDialogueGraphEditorApp> InApp)
{
	DialogueGraphApp = InApp;
	UDialogueGraphProjectSettings* Settings = UDialogueGraphProjectSettings::Get();
	
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

	const TSharedPtr<SScrollBox> PreviewBox = SNew(SScrollBox);

	PreviewBox->AddSlot()
	.Padding(10.0f)
	[
		SAssignNew(SpeakerBox, SBorder)
		.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
		.BorderBackgroundColor(FLinearColor(.36f, 0.36f, 0.36f, 0.25f))
		.Padding(FMargin(10.0f))
		.Visibility(EVisibility::Collapsed)
		[
			SAssignNew(SpeakerRichTextBlock, SRichTextBlock)
			.Text(FText::GetEmpty())
			.DecoratorStyleSet(Settings->GetRichTextStyleSet().Get())
			.TextStyle(&Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->SpeakerPreviewStyle))
			.Decorators(Decorators)
			.AutoWrapText(true)
		]
	];

	PreviewBox->AddSlot()
	.Padding(10.0f)
	[
		SAssignNew(DialogueBox, SBorder)
		.BorderBackgroundColor(FLinearColor(0,0,0,0))
		.Visibility(EVisibility::Collapsed)
		[
			SAssignNew(DialogueRichTextBlock, SRichTextBlock)
			.Text(FText::GetEmpty())
			.DecoratorStyleSet(Settings->GetRichTextStyleSet().Get())
			.TextStyle(&Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->DialoguePreviewStyle))
			.Decorators(Decorators)
			.AutoWrapText(true)
		]
	];
	
	SAssignNew(OptionsBox, SVerticalBox);

	PreviewBox->AddSlot()
	.Padding(10.0f)
	[
		OptionsBox.ToSharedRef()
	];

	ChildSlot
	[
		PreviewBox.ToSharedRef()
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

	if (const TSharedPtr<FDialogueGraphEditorApp> App = DialogueGraphApp.Pin()) {
		App->OnGraphSelectionChangedDelegate.Remove(SelectionChangedHandle);
	}
	
	if (UDialogueGraphProjectSettings* Settings = UDialogueGraphProjectSettings::Get()) {
		Settings->OnRichTextStyleChanged.Remove(StyleChangedHandle);
	}
}

void SDialoguePreviewTab::ProcessTextNode(FText& NewPreviewText, FText& NewSpeakerPreviewText, const UDialogueGraphNodeText* TextNode)
{
	if (UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(TextNode->GetNodeInfo())) {
		const FString Language = UDialogueGraphProjectSettings::Get()->GetPreviewLanguage();
				
		CurrentNode = NodeInfo;
				
		if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable && !NodeInfo->DialogueKey.IsNone()) {
			FDataTableRowHandle Handle;
			Handle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
			Handle.RowName = NodeInfo->DialogueKey;
			NewPreviewText = FText::FromString(UGSheetLocSystemLibrary::GetLocalizedStringManual(Handle, Language));

			FString CombinedSpeakers;
			for (const FName& ID : NodeInfo->SpeakerKeys) {
				if (ID.IsNone()) 
					continue;
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
		else if (!NodeInfo->DialogueKey.IsNone()) {
			NewPreviewText = FText::FromString(NodeInfo->DialogueKey.ToString());
		}
				
		PropertyChangedHandle = NodeInfo->OnPropertiesChanged.AddRaw(this, &SDialoguePreviewTab::RefreshPreview);
	}
}

void SDialoguePreviewTab::ProcessBranchNode(const UDialogueGraphNodeBranch* BranchNode)
{
	if (UDialogueNodeInfoBranch* NodeInfo = Cast<UDialogueNodeInfoBranch>(BranchNode->GetNodeInfo())) {
		const FString Language = UDialogueGraphProjectSettings::Get()->GetPreviewLanguage();
				
		CurrentNode = NodeInfo;
				
		if (NodeInfo->DialogueSystem && NodeInfo->DialogueSystem->DialogueDataTable && !NodeInfo->BranchOptions.IsEmpty()) {
			for (FBranchCondition ID : NodeInfo->BranchOptions) {
				if (ID.DialogueResponseKey.IsNone()) 
					continue;
				if (NodeInfo->DialogueSystem->DialogueDataTable) {
					FDataTableRowHandle OptionHandle;
					OptionHandle.DataTable = NodeInfo->DialogueSystem->DialogueDataTable;
					OptionHandle.RowName = ID.DialogueResponseKey;
					NewOptionsPreviewTexts.Add(UGSheetLocSystemLibrary::GetLocalizedStringManual(OptionHandle, Language));
				}
				else {
					NewOptionsPreviewTexts.Add(ID.DialogueResponseKey.ToString());
				}
			}
		}
				
		PropertyChangedHandle = NodeInfo->OnPropertiesChanged.AddRaw(this, &SDialoguePreviewTab::RefreshPreview);
	}
}

void SDialoguePreviewTab::OnGraphSelectionChanged(const FGraphPanelSelectionSet& InSelectionSet)
{
	CachedSelection = InSelectionSet;
	
	FText NewPreviewText = FText::GetEmpty();
	FText NewSpeakerPreviewText = FText::GetEmpty();
	NewOptionsPreviewTexts.Empty();
	
	if (CurrentNode.IsValid()) {
		CurrentNode->OnPropertiesChanged.Remove(PropertyChangedHandle);
	}
	CurrentNode.Reset();
	
	for (UObject* Obj : InSelectionSet) {
		if (const UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(Obj)) {
			ProcessTextNode(NewPreviewText, NewSpeakerPreviewText, TextNode);
			break;
		}
		if (const UDialogueGraphNodeBranch* Branch = Cast<UDialogueGraphNodeBranch>(Obj)) {
			ProcessBranchNode(Branch);
			break;
		}
	}

	if (DialogueRichTextBlock.IsValid() && DialogueBox.IsValid()) {
		DialogueRichTextBlock->SetText(NewPreviewText);
		DialogueBox->SetVisibility(NewSpeakerPreviewText.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible);
	}
	if (SpeakerRichTextBlock.IsValid() && SpeakerBox.IsValid()) {
		SpeakerRichTextBlock->SetText(NewSpeakerPreviewText);
		SpeakerBox->SetVisibility(NewSpeakerPreviewText.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible);
	}

	if (OptionsBox.IsValid()) {
		OptionsBox->ClearChildren();
		if (UDialogueGraphProjectSettings* Settings = UDialogueGraphProjectSettings::Get()) {
			for (int32 i = 0; i < NewOptionsPreviewTexts.Num(); ++i) {
				OptionsBox->AddSlot()
				.AutoHeight()
				.Padding(0.0f, 2.0f, 0.0f, 2.0f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
					.BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.05f, 0.5f))
					.Padding(FMargin(5.0f))
					[
						SNew(SRichTextBlock)
						.Text(FText::FromString(NewOptionsPreviewTexts[i]))
						.DecoratorStyleSet(Settings->GetRichTextStyleSet().Get())
						.TextStyle(&Settings->GetRichTextStyleSet()->GetWidgetStyle<FTextBlockStyle>(Settings->DialoguePreviewStyle))
						.Decorators(CachedDecorators)
						.AutoWrapText(true)
					]
				];
			}
		}
	}
}

void SDialoguePreviewTab::RefreshPreview()
{
	if (CachedSelection.IsEmpty()) return;
	OnGraphSelectionChanged(CachedSelection);
}

void SDialoguePreviewTab::OnRichTextStyleChanged() const
{
	if (UDialogueGraphProjectSettings* Settings = UDialogueGraphProjectSettings::Get()) {
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
