// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

/**
 * Editor application for managing and editing dialogue graphs.
 * Provides functionality for graph editing, node selection, and property updates.
 * Implements workflow-centric application features with undo/redo support and notification hooks.
 * Manages the visual representation and editing of dialogue systems through a graph interface.
 */
class DialogueGraphEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	
	/**
	 * Registers tab spawners with the provided tab manager.
	 * Sets up the available tabs for the dialogue graph editor interface.
	 * 
	 * @param TabManagerRef Reference to the tab manager that will handle the registered spawners
	 */
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerRef) override;

	/**
	 * Initializes the dialogue graph editor with specified parameters.
	 * Sets up the editor interface and prepares it for editing the provided object.
	 * 
	 * @param Mode The toolkit mode that determines how the editor should be initialized
	 * @param InitToolkitHost The toolkit host that will contain this editor
	 * @param ObjectToEdit The dialogue system object that will be edited in this editor instance
	 */
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	class UDialogueSystem* GetDialogueGraph() const { return WorkingAsset; }
	UEdGraph* GetGraphEditor() const { return WorkingGraphEditor; }

	void SetWorkingGraphUi(const TSharedPtr<SGraphEditor>& WorkingGraphUi) { WorkingGraphUiPtr = WorkingGraphUi; }

	/**
	 * Sets the detail view for displaying selected node properties.
	 * Updates the reference to the detail view widget used for showing and editing node properties.
	 *
	 * @param SelectedNodeDetailView Shared pointer to the details view widget to be used
	 */
	void SetSelectedNodeDetailView(const TSharedPtr<IDetailsView>& SelectedNodeDetailView);

	/**
	 * Called when the selection in the graph editor changes.
	 * Updates the node detail view with properties of the newly selected node.
	 * 
	 * @param SelectionSet Set of currently selected graph elements
	 */
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectionSet) const;

	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("DialogueGraphEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("DialogueGraphEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("DialogueGraphEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.15f, 0.2f, 0.5f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return TEXT("doc link"); }

	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}

	virtual void OnClose() override;
	void OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event);
	void OnWorkingGraphAssetPreSave();

protected:
	
	/**
	 * Updates the working asset with the current state of the graph editor.
	 * Synchronizes changes made in the graph editor UI with the underlying asset data structure.
	 * This ensures that visual changes in the editor are properly reflected in the asset.
	 */
	void UpdateWorkingAssetFromGraph() const;
	
	/**
	 * Updates the graph editor UI with the current state of the working asset.
	 * Synchronizes the visual representation in the editor with the underlying asset data.
	 * This ensures that changes in the asset are properly reflected in the graph editor.
	 */
	void UpdateGraphEditorFromWorkingAsset() const;
	
	/**
	 * Retrieves the currently selected node from the graph editor.
	 * @param SelectionSet The set of currently selected graph elements
	 * @return Pointer to the selected dialogue graph node, or nullptr if no valid node is selected
	 */
	static class UDialogueGraphNodeBase* GetSelectedNode(const FGraphPanelSelectionSet& SelectionSet);

private:
	// This property holds the reference to the currently edited asset.
	UPROPERTY()
	UDialogueSystem* WorkingAsset = nullptr;

	// This property holds the reference to the currently edited graph in the editor.
	UPROPERTY()
	UEdGraph* WorkingGraphEditor = nullptr;

	// The slate widget for the graph editor
	TSharedPtr<SGraphEditor> WorkingGraphUiPtr = nullptr;

	// The slate widget with details of the selected node
	TSharedPtr<IDetailsView> SelectedNodeDetailViewPtr = nullptr;
};
