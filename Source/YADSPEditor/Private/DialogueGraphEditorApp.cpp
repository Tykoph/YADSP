// Copyright Tom Duby. All Rights Reserved.

#include "DialogueGraphEditorApp.h"
#include "DialogueGraphSchema.h"
#include "DialogueGraphSettings.h"
#include "Compiler/DialogueGraphCompiler.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeBase.h"
#include "DialogueSystemAppMode.h"
#include "DialogueSystem.h"
#include "YADSP.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Input/SComboBox.h"

void FDialogueGraphEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerRef);
}

void FDialogueGraphEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	// Initialize the working asset and graph editor
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	WorkingAsset = Cast<UDialogueSystem>(InObject);
	if (WorkingAsset == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("DialogueGraphEditorApp::InitEditor -> Working Asset is null ptr"));
		return;
	}
	
	WorkingAsset->SetPreSaveListener([this]() { OnWorkingGraphAssetPreSave(); });

	WorkingGraphEditor = FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UDialogueGraphSchema::StaticClass()
	);

	// Initialize the asset editor
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
	const TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
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
					.OptionsSource(UDialogueGraphSettings::Get()->GetLanguageOptions())
					.OnGenerateWidget_Lambda([](const TSharedPtr<FString>& Item)
					{
						return SNew(STextBlock).Text(FText::FromString(*Item));
					})
					.OnSelectionChanged_Lambda([this](const TSharedPtr<FString>& NewSelection, ESelectInfo::Type SelectInfo)
					{
						if (NewSelection.IsValid()) {
							UDialogueGraphSettings::SetPreviewLanguage(*NewSelection);
						}
					})
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							return FText::FromString(UDialogueGraphSettings::Get()->GetPreviewLanguage());
						})
					]
				]
			);
		})
	);
	AddToolbarExtender(ToolbarExtender);
	
	// Set the current mode to the DialogueGraphAppMode
	AddApplicationMode(TEXT("DialogueGraphAppMode"), MakeShareable(new FDialogueSystemAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("DialogueGraphAppMode"));

	// Update the graph editor from the working asset
	FDialogueGraphCompiler::UpdateGraphEditorFromWorkingAsset(WorkingAsset, WorkingGraphEditor);

	UDialogueGraphSettings::Get()->OnPreviewLanguageChanged.AddSP(this, &FDialogueGraphEditorApp::OnLanguageChanged);
}

void FDialogueGraphEditorApp::OnLanguageChanged() const
{
	if (WorkingGraphUI.IsValid()) {
		WorkingGraphUI->NotifyGraphChanged();
	}
}


void FDialogueGraphEditorApp::OnClose()
{
	FDialogueGraphCompiler::UpdateWorkingAssetFromGraph(WorkingAsset, WorkingGraphEditor);
	WorkingAsset->SetPreSaveListener(nullptr);
	FAssetEditorToolkit::OnClose();
}

// Called when the properties of the node detail view change
void FDialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event) const
{
	if (WorkingGraphUI != nullptr) {
		// Get the node getting modified
		UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(WorkingGraphUI->GetSelectedNodes());
		if (SelectedNode != nullptr) {
			// Call OnPropertiesChanged() on the selected node
			SelectedNode->OnPropertiesChanged();
		}

		// Notify the graph UI that the graph has changed
		WorkingGraphUI->NotifyGraphChanged();
	}
}

void FDialogueGraphEditorApp::OnWorkingGraphAssetPreSave() const
{
	FDialogueGraphCompiler::UpdateWorkingAssetFromGraph(WorkingAsset, WorkingGraphEditor);
}

UDialogueGraphNodeBase* FDialogueGraphEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& InSelectionSet)
{
	for (UObject* Obj : InSelectionSet) {
		UDialogueGraphNodeBase* SelectedNode = Cast<UDialogueGraphNodeBase>(Obj);
		if (SelectedNode != nullptr) {
			return SelectedNode;
		}
	}
	
	return nullptr;
}

void FDialogueGraphEditorApp::SetSelectedNodeDetailView(const TSharedPtr<IDetailsView>& InSelectedNodeDetailView)
{
	SelectedNodeDetailView = InSelectedNodeDetailView;
	if (!SelectedNodeDetailView.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("FDialogueGraphEditorApp::SetSelectedNodeDetailView -> SelectedNodeDetailView is not valid"));
		return;
	}
	SelectedNodeDetailView->OnFinishedChangingProperties().AddRaw(this, &FDialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated);
}

void FDialogueGraphEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& InSelectionSet) const
{
	if (!SelectedNodeDetailView.IsValid()) {
		UE_LOG(LogYADSP, Error, TEXT("FDialogueGraphEditorApp::OnGraphSelectionChanged -> SelectedNodeDetailView is not valid"));
	}
	
	const UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(InSelectionSet);
	if (SelectedNode != nullptr) {
		SelectedNodeDetailView->SetObject(SelectedNode->GetNodeInfo());
	}
	else {
		SelectedNodeDetailView->SetObject(nullptr);
	}

	OnGraphSelectionChangedDelegate.Broadcast(InSelectionSet);
}
