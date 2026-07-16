// Copyright Tom Duby. All Rights Reserved.

#include "UI/SDialogueGraphDetailsTab.h"
#include "DialogueGraphEditorApp.h"
#include "DialogueGraphUserSettings.h"
#include "DialogueSystem.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"

void SDialogueGraphDetailsTab::Construct(const FArguments& InArgs, TSharedPtr<FDialogueGraphEditorApp> InApp)
{
	DialogueGraphApp = InApp;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	// Detail view args
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bSearchInitialKeyFocus = true;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.NotifyHook = nullptr;
	DetailsViewArgs.bShowOptions = true;
	DetailsViewArgs.bShowModifiedPropertiesOption = false;
	DetailsViewArgs.bShowScrollBar = false;

	const TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(InApp->GetDialogueGraph());

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 10.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Preview Background:"))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SColorBlock)
				.Color_Lambda([]() { return UDialogueGraphUserSettings::Get()->PreviewBackgroundColor; })
				.ShowBackgroundForAlpha(true)
				.OnMouseButtonDown(this, &SDialogueGraphDetailsTab::OnColorBlockClicked)
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			DetailsView.ToSharedRef()
		]
	];
}

FReply SDialogueGraphDetailsTab::OnColorBlockClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		FColorPickerArgs PickerArgs;
		PickerArgs.bUseAlpha = true;
		PickerArgs.InitialColor = UDialogueGraphUserSettings::Get()->PreviewBackgroundColor;
        
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateLambda([](FLinearColor NewColor) {
			UDialogueGraphUserSettings* Settings = UDialogueGraphUserSettings::Get();
			Settings->PreviewBackgroundColor = NewColor;
			Settings->SaveConfig();
		});
        
		OpenColorPicker(PickerArgs);
		return FReply::Handled();
	}
    
	return FReply::Unhandled();
}