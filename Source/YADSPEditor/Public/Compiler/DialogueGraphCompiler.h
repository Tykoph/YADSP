#pragma once
#include "DialogueSystem.h"

class DialogueGraphCompiler
{
public:
	/**
	 * Updates the working asset with the current state of the graph editor.
	 * Synchronizes changes made in the graph editor UI with the underlying asset data structure.
	 * This ensures that visual changes in the editor are properly reflected in the asset.
	 */
	static void UpdateWorkingAssetFromGraph(UDialogueSystem* WorkingAsset, UEdGraph* WorkingGraphEditor);

	/**
	 * Updates the graph editor UI with the current state of the working asset.
	 * Synchronizes the visual representation in the editor with the underlying asset data.
	 * This ensures that changes in the asset are properly reflected in the graph editor.
	 */
	static void UpdateGraphEditorFromWorkingAsset(UDialogueSystem* WorkingAsset, UEdGraph* WorkingGraphEditor);
};
