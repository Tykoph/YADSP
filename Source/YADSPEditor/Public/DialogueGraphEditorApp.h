#pragma once

#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class YADSPEDITOR_API DialogueGraphEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	class UDialogueSystem* GetDialogueGraph() const { return WorkingAsset; }
	class UEdGraph* GetGraphEditor() const { return WorkingGraphEditor; }

	void SetWorkingGraphUi(TSharedPtr<SGraphEditor> WorkingGraphUi) {WorkingGraphUiPtr = WorkingGraphUi;}
	void SetSelectedNodeDetailView(TSharedPtr<IDetailsView> SelectedNodeDetailView);
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet);

public:	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("DialogueGraphEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("DialogueGraphEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("DialogueGraphEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.15f, 0.2f, 0.5f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return TEXT("doc link"); }
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}

	virtual void OnClose() override;
	void OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event);
	void OnWorkingGraphAssetPreSave();

protected:
	void UpdateWorkingAssetFromGraph();
	void UpdateGraphEditorFromWorkingAsset();
	class UDialogueGraphNodeBase* GetSelectedNode(const FGraphPanelSelectionSet& SelectionSet);

private:
	UPROPERTY()
	class UDialogueSystem* WorkingAsset = nullptr;

	UPROPERTY()
	class UEdGraph* WorkingGraphEditor = nullptr;

	// the slate widget for the graph editor
	TSharedPtr<SGraphEditor> WorkingGraphUiPtr = nullptr;

	TSharedPtr<IDetailsView> SelectedNodeDetailViewPtr = nullptr;
};
