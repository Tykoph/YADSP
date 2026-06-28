// Copyright Tom Duby. All Rights Reserved.

#pragma once
#include "DialogueSystem.h"

class FDialogueGraphCompiler
{
public:
	/**
	 * Updates the working asset with the current state of the graph editor.
	 * Synchronizes changes made in the graph editor UI with the underlying asset data structure.
	 * This ensures that visual changes in the editor are properly reflected in the asset.
	 */
	static void UpdateWorkingAssetFromGraph(UDialogueSystem* InWorkingAsset, UEdGraph* InWorkingGraphEditor);

	/**
	 * Updates the graph editor UI with the current state of the working asset.
	 * Synchronizes the visual representation in the editor with the underlying asset data.
	 * This ensures that changes in the asset are properly reflected in the graph editor.
	 */
	static void UpdateGraphEditorFromWorkingAsset(UDialogueSystem* InWorkingAsset, UEdGraph* InWorkingGraphEditor);
};
