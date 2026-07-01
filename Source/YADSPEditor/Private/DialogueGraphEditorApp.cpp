// Copyright Tom Duby. All Rights Reserved.

#include "DialogueGraphEditorApp.h"

#include "DialogueGraphCommands.h"
#include "DialogueGraphSchema.h"
#include "DialogueGraphProjectSettings.h"
#include "DialogueGraphUserSettings.h"
#include "Compiler/DialogueGraphCompiler.h"
#include "Nodes/DialogueGraphNodeBase.h"
#include "DialogueSystemAppMode.h"
#include "DialogueSystem.h"
#include "YADSP.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Nodes/DialogueGraphNodeBranch.h"
#include "Nodes/DialogueGraphNodeEnd.h"
#include "Nodes/DialogueGraphNodeGameAction.h"
#include "Nodes/DialogueGraphNodeGoTo.h"
#include "Nodes/DialogueGraphNodeLabel.h"
#include "Nodes/DialogueGraphNodeText.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

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
	
	// keyboard Shortcut
	FDialogueGraphCommands::Register();
	BindCommands();
	
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
			Builder.AddWidget(
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(4.0f, 0.0f)
				.VAlign(VAlign_Center)
				[
					SNew(SCheckBox)
					.IsChecked_Lambda([]() { return UDialogueGraphUserSettings::Get()->bDisplayRichTextFlags ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
					.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) {
						UDialogueGraphUserSettings* Settings = UDialogueGraphUserSettings::Get();
						Settings->bDisplayRichTextFlags = (NewState == ECheckBoxState::Checked);
						Settings->SaveConfig();
						if (WorkingGraphUI.IsValid()) {
							WorkingGraphUI->NotifyGraphChanged();
						}
					})
					[
						SNew(STextBlock).Text(FText::FromString("Show Rich Text Flags"))
					]
				]
			);
			Builder.AddSeparator();
			Builder.AddWidget(
				SNew(SBox)
				.WidthOverride(100.0f)
				[
					SNew(SComboBox<TSharedPtr<FString>>)
					.OptionsSource(UDialogueGraphProjectSettings::Get()->GetLanguageOptions())
					.OnGenerateWidget_Lambda([](const TSharedPtr<FString>& Item)
					{
						return SNew(STextBlock).Text(FText::FromString(*Item));
					})
					.OnSelectionChanged_Lambda([this](const TSharedPtr<FString>& NewSelection, ESelectInfo::Type SelectInfo)
					{
						if (NewSelection.IsValid()) {
							UDialogueGraphProjectSettings::SetPreviewLanguage(*NewSelection);
						}
					})
					[
						SNew(STextBlock)
						.Text_Lambda([this]()
						{
							return FText::FromString(UDialogueGraphProjectSettings::Get()->GetPreviewLanguage());
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

	UDialogueGraphProjectSettings::Get()->OnPreviewLanguageChanged.AddSP(this, &FDialogueGraphEditorApp::OnLanguageChanged);
	UDialogueGraphUserSettings::Get()->OnShortcutsChanged.AddSP(this, &FDialogueGraphEditorApp::UpdateShortcuts);
	UpdateShortcuts(); 
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

void FDialogueGraphEditorApp::OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event) const
{
	if (WorkingGraphUI != nullptr) {
		UDialogueGraphNodeBase* SelectedNode = GetSelectedNode(WorkingGraphUI->GetSelectedNodes());
		if (SelectedNode != nullptr) {
			SelectedNode->OnPropertiesChanged();
		}

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

void FDialogueGraphEditorApp::BindCommands()
{
	GraphEditorCommands = MakeShareable(new FUICommandList);

	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().CreateTextNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnCreateNode, UDialogueGraphNodeText::StaticClass())
	);
	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().CreateBranchNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnCreateNode, UDialogueGraphNodeBranch::StaticClass())
	);
	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().CreateGameActionNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnCreateNode, UDialogueGraphNodeGameAction::StaticClass())
	);
	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().CreateGoToNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnCreateNode, UDialogueGraphNodeGoTo::StaticClass())
	);
	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().CreateLabelNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnCreateNode, UDialogueGraphNodeLabel::StaticClass())
	);
	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().CreateEndNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnCreateNode, UDialogueGraphNodeEnd::StaticClass())
	);
	
	GraphEditorCommands->MapAction(
		FDialogueGraphCommands::Get().DeleteNode,
		FExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::OnDeleteNodes),
		FCanExecuteAction::CreateSP(this, &FDialogueGraphEditorApp::CanDeleteNodes)
	);
}

void FDialogueGraphEditorApp::UpdateShortcuts()
{
	if (const UDialogueGraphUserSettings* Settings = UDialogueGraphUserSettings::Get()) {
		FDialogueGraphCommands::Get().CreateTextNode->SetActiveChord(Settings->CreateTextNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
		FDialogueGraphCommands::Get().CreateBranchNode->SetActiveChord(Settings->CreateBranchNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
		FDialogueGraphCommands::Get().CreateGameActionNode->SetActiveChord(Settings->CreateGameActionNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
		FDialogueGraphCommands::Get().CreateGoToNode->SetActiveChord(Settings->CreateGoToNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
		FDialogueGraphCommands::Get().CreateLabelNode->SetActiveChord(Settings->CreateLabelNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
		FDialogueGraphCommands::Get().CreateEndNode->SetActiveChord(Settings->CreateEndNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
		FDialogueGraphCommands::Get().DeleteNode->SetActiveChord(Settings->DeleteNodeShortcut.GetInputChord(), EMultipleKeyBindingIndex::Primary);
	}
}

void FDialogueGraphEditorApp::OnCreateNode(UClass* NodeClass) const
{
	if (WorkingGraphEditor != nullptr) {
		const FVector2D SpawnLocation = WorkingGraphUI->GetPasteLocation();
		UEdGraphPin* FromPin = nullptr;
		FNewNodeAction NodeAction(NodeClass, FText::GetEmpty(), FText::GetEmpty(), FText::GetEmpty(), 0);
		NodeAction.PerformAction(WorkingGraphEditor, FromPin, SpawnLocation, true);
	}
}

void FDialogueGraphEditorApp::OnDeleteNodes() const
{
	if (WorkingGraphUI.IsValid()) {
		const FGraphPanelSelectionSet SelectedNodes = WorkingGraphUI->GetSelectedNodes();
		for (UObject* Node : SelectedNodes) {
			if (UEdGraphNode* GraphNode = Cast<UEdGraphNode>(Node)) {
				GraphNode->GetGraph()->RemoveNode(GraphNode);
			}
		}
		
		WorkingGraphUI->ClearSelectionSet();
		WorkingGraphUI->NotifyGraphChanged();
	}
}

bool FDialogueGraphEditorApp::CanDeleteNodes() const
{
	return WorkingGraphUI.IsValid() && WorkingGraphUI->GetSelectedNodes().Num() > 0;
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
