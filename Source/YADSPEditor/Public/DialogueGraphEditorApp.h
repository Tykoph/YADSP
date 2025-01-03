﻿#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"


class YADSPEDITOR_API DialogueGraphEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{

private:
	UPROPERTY()
	class UDialogueSystem* WorkingGraphAsset = nullptr;

	UPROPERTY()
	class UEdGraph* WorkingGraphEditor = nullptr;

	FDelegateHandle OnGraphChangedListenerHandle;

public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& tabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit);

	UDialogueSystem* GetDialogueGraph() const { return WorkingGraphAsset; }
	UEdGraph* GetGraphEditor() const { return WorkingGraphEditor; }

	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("DialogueGraphEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("DialogueGraphEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("DialogueGraphEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.15f, 0.2f, 0.5f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return TEXT("doc link"); }
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnClose() override;
	void OnGraphChanged(const FEdGraphEditAction& Action);

protected:
	void UpdateWorkingAssetFromGraph();
	void UpdateGraphEditorFromWorkingAsset();
};
