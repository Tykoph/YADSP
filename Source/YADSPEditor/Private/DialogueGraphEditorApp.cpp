// Copyright 2026 Tom Duby. All Rights Reserved.

#include "DialogueGraphEditorApp.h"
#include "DialogueSystemAppMode.h"
#include "DialogueSystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogueGraphSchema.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeStart.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeText.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeAction.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeAnimation.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeEnd.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeBase.h"
#include "GSheetLocSystemDefinitions.h"
#include "K2Node_GetDataTableRow.h"
#include "Compiler/DialogueGraphCompiler.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Input/SComboBox.h"

void DialogueGraphEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void DialogueGraphEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	// initialize the working asset and graph editor
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	WorkingAsset = Cast<UDialogueSystem>(InObject);
	WorkingAsset->SetPreSaveListener([this]() { OnWorkingGraphAssetPreSave(); });

	WorkingGraphEditor = FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UDialogueGraphSchema::StaticClass()
	);

	// initialize the asset editor
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		TEXT("DialogueGraphEditorApp"),
		FTabManager::FLayout::NullLayout,
		true, // bCreateDefaultStandaloneMenu
		true, // bCreateDefaultToolbar
		ObjectsToEdit
	);

	// Add Toolbar Extension for Language Selection
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& Builder)
		{
			Builder.AddSeparator();
			Builder.AddWidget(
				SNew(SBox)
				.WidthOverride(100.0f)
				[
					SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(&LanguageOptions)
					.OnGenerateWidget_Lambda([](const TSharedPtr<FString>& Item)
					{
						return SNew(STextBlock).Text(FText::FromString(*Item));
					})
					.OnSelectionChanged_Lambda([this](const TSharedPtr<FString>& NewSelection, ESelectInfo::Type SelectInfo)
					{
						if (NewSelection.IsValid()) {
							SetPreviewLanguage(*NewSelection);
						}
					})
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							return FText::FromString(PreviewLanguage);
						})
					]
				]
			);
		})
	);
	AddToolbarExtender(ToolbarExtender);

	// Initialize Language Options
	LanguageOptions.Add(MakeShared<FString>(TEXT("zh-Hans")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-150")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("en-US")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("fr")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("ja")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("pt-BR")));
	LanguageOptions.Add(MakeShared<FString>(TEXT("es-ES")));

	// set the current mode to the DialogueGraphAppMode
	AddApplicationMode(TEXT("DialogueGraphAppMode"), MakeShareable(new DialogueSystemAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("DialogueGraphAppMode"));

	// update the graph editor from the working asset
	DialogueGraphCompiler::UpdateGraphEditorFromWorkingAsset(WorkingAsset, WorkingGraphEditor);
}

void DialogueGraphEditorApp::SetPreviewLanguage(const FString& NewLanguage)
{
	if (PreviewLanguage != NewLanguage) {
		PreviewLanguage = NewLanguage;
		if (WorkingGraphUiPtr.IsValid()) {
			WorkingGraphUiPtr->NotifyGraphChanged();
		}
	}
}

void DialogueGraphEditorApp::OnClose()
{
	DialogueGraphCompiler::UpdateWorkingAssetFromGraph(WorkingAsset, WorkingGraphEditor);
	WorkingAsset->SetPreSaveListener(nullptr);
	FAssetEditorToolkit::OnClose();
}

// Called when the properties of the node detail view change
void DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event) const
{
	if (WorkingGraphUiPtr != nullptr) {
		//get the node getting modified
		UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(WorkingGraphUiPtr->GetSelectedNodes());
		if (SelectedNode != nullptr) {
			// Call OnPropertiesChanged() on the selected node
			SelectedNode->OnPropertiesChanged();
		}

		// Notify the graph UI that the graph has changed
		WorkingGraphUiPtr->NotifyGraphChanged();
	}
}

void DialogueGraphEditorApp::OnWorkingGraphAssetPreSave() const
{
	DialogueGraphCompiler::UpdateWorkingAssetFromGraph(WorkingAsset, WorkingGraphEditor);
}

UDialogueGraphNodeBase* DialogueGraphEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& SelectionSet)
{
	for (UObject* Obj : SelectionSet) {
		UDialogueGraphNodeBase* SelectedNode = Cast<UDialogueGraphNodeBase>(Obj);
		if (SelectedNode != nullptr) {
			return SelectedNode;
		}
	}

	return nullptr;
}

void DialogueGraphEditorApp::SetSelectedNodeDetailView(const TSharedPtr<IDetailsView>& SelectedNodeDetailView)
{
	SelectedNodeDetailViewPtr = SelectedNodeDetailView;
	SelectedNodeDetailViewPtr->OnFinishedChangingProperties().AddRaw(
		this, &DialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated);
}

void DialogueGraphEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet) const
{
	UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(SelectionSet);
	if (SelectedNode != nullptr) {
		SelectedNodeDetailViewPtr->SetObject(SelectedNode->GetNodeInfo());
	}
	else {
		SelectedNodeDetailViewPtr->SetObject(nullptr);
	}
}
