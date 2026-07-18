// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

/** Delegate fired when the graph panel selection changes. */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGraphSelectionChangedDelegate, const FGraphPanelSelectionSet&);

/**
 * Editor application for managing and editing dialogue graphs.
 * Provides functionality for graph editing, node selection, and property updates.
 * Implements workflow-centric application features with undo/redo support and notification hooks.
 * Manages the visual representation and editing of dialogue systems through a graph interface.
 */
class YADSPEDITOR_API FDialogueGraphEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	/** Registers the tab spawners for the dialogue graph editor. */
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

	/** Gets the dialogue system working asset. */
	class UDialogueSystem* GetDialogueGraph() const { return WorkingAsset; }
	/** Gets the working graph editor. */
	UEdGraph* GetGraphEditor() const { return WorkingGraphEditor; }

	/** Sets the slate widget used for the graph editor UI. */
	void SetWorkingGraphUi(const TSharedPtr<SGraphEditor>& InWorkingGraphUI) { WorkingGraphUI = InWorkingGraphUI; }

	/**
	 * Sets the detail view for displaying selected node properties.
	 * Updates the reference to the detail view widget used for showing and editing node properties.
	 *
	 * @param InSelectedNodeDetailView Shared pointer to the details view widget to be used
	 */
	void SetSelectedNodeDetailView(const TSharedPtr<IDetailsView>& InSelectedNodeDetailView);

	/** Invoked when graph selection is updated. */
	FOnGraphSelectionChangedDelegate OnGraphSelectionChangedDelegate;

	/**
	 * Called when the selection in the graph editor changes.
	 * Updates the node detail view with properties of the newly selected node.
	 * 
	 * @param InSelectionSet Set of currently selected graph elements
	 */
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& InSelectionSet) const;

	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("DialogueGraphEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("DialogueGraphEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("DialogueGraphEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.15f, 0.2f, 0.5f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return TEXT("doc link"); }

	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}

	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}

	/** Called when the application is closing. */
	virtual void OnClose() override;
	
	/**
	 * Handles property change events from the node details view.
	 * @param Event The property changed event data.
	 */
	void OnNodeDetailViewPropertiesUpdated(const FPropertyChangedEvent& Event) const;
	
	/** Invoked prior to saving the working graph asset to sync state. */
	void OnWorkingGraphAssetPreSave() const;
	
	/** Returns the list of UI commands for the graph editor. */
	TSharedRef<FUICommandList> GetGraphEditorCommands() const { return GraphEditorCommands.ToSharedRef(); }

	// FEditorUndoClient interface
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;
	// /////////

protected:
	/**
	 * Retrieves the currently selected node from the graph editor.
	 * @param InSelectionSet The set of currently selected graph elements
	 * @return Pointer to the selected dialogue graph node, or nullptr if no valid node is selected
	 */
	static class UDialogueGraphNodeBase* GetSelectedNode(const FGraphPanelSelectionSet& InSelectionSet);

	/** Binds all actions to their respective commands. */
	void BindCommands();
	
	/** Updates editor shortcuts from user settings. */
	void UpdateShortcuts();
	
	/** Creates a new node of the given class. */
	void OnCreateNode(UClass* NodeClass) const;
	
	/** Deletes currently selected nodes. */
	void OnDeleteNodes() const;
	
	/** Checks if any of the selected nodes can be deleted. */
	bool CanDeleteNodes() const;
	
	/** Checks if any of the selected nodes can be copied to the clipboard. */
	bool CanCopyNodes() const;
	
	/** Copies the currently selected nodes to the clipboard. */
	void OnCopyNodes() const;
	
	/** Checks if valid nodes exist in the clipboard to be pasted. */
	bool CanPasteNodes() const;
	
	/** Pastes nodes from the clipboard into the current graph. */
	void OnPasteNodes() const;
	
	/** Checks if any of the selected nodes can be cut from the graph. */
	bool CanCutNodes() const;
	
	/** Cuts the currently selected nodes, copying them to the clipboard and removing them from the graph. */
	void OnCutNodes() const;
	
	/** Checks if any of the selected nodes can be duplicated. */
	bool CanDuplicateNodes() const;
	
	/** Duplicates the currently selected nodes in the graph. */
	void OnDuplicateNodes() const;
	
	TSharedPtr<FUICommandList> GraphEditorCommands;
	
private:
	void OnLanguageChanged() const;
	
	// This property holds the reference to the currently edited asset.
	UDialogueSystem* WorkingAsset = nullptr;

	// This property holds the reference to the currently edited graph in the editor.
	UEdGraph* WorkingGraphEditor = nullptr;

	// The slate widget for the graph editor
	TSharedPtr<SGraphEditor> WorkingGraphUI = nullptr;

	// The slate widget with details of the selected node
	TSharedPtr<IDetailsView> SelectedNodeDetailView = nullptr;
};
