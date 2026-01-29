// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueGraphEditorApp.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogueNodeType.h"
#include "DialogueGraphNodeBase.h"
#include "DialogueNodeInfoText.h"
#include "SGraphNode.h"
#include "DialogueGraphNodeText.Generated.h"

UCLASS()
class UDialogueGraphNodeText : public UDialogueGraphNodeBase
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	// UDialogueGraphNodeBase interface
	virtual UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name) override;
	virtual UEdGraphPin* CreateDefaultInputPin() override;
	virtual void CreateDefaultOutputPin() override;

	virtual EDialogueNodeType GetNodeType() const override { return EDialogueNodeType::TextNode; }
	virtual void OnPropertiesChanged() override { SyncWithNodeResponse(); }

	// Our Interface
	void SyncWithNodeResponse();

	virtual void InitNodeInfo(UObject* Output) override { NodeInfoPtr = NewObject<UDialogueNodeInfoText>(Output); }

	virtual void SetNodeInfo(UDialogueNodeInfoBase* NodeInfo) override
	{
		NodeInfoPtr = Cast<UDialogueNodeInfoText>(NodeInfo);
	}

	virtual UDialogueNodeInfoBase* GetNodeInfo() const override { return NodeInfoPtr; }
	UDialogueNodeInfoText* GetDialogueNodeInfo() const { return NodeInfoPtr; }

protected:
	UPROPERTY()
	UDialogueNodeInfoText* NodeInfoPtr = nullptr;
};

//
// Custom node format
//
class SDialogueGraphNodeText : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphNodeText)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UDialogueGraphNodeText* InNode);
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;
	virtual void UpdateGraphNode() override;

protected:
	FText GetPreviewSpeakerText() const;
	FText GetPreviewDialogueText() const;

	// Helper to get the hosting editor app to access settings like Language
	TSharedPtr<DialogueGraphEditorApp> GetGraphEditorApp() const;

	TSharedPtr<FString> CurrentSpeakerSelection;
	TSharedPtr<FString> CurrentDialogueSelection;

	void OnDialogueSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	FText GetDialogueComboText() const;

	// Multiple Speakers Support
	TSharedPtr<SVerticalBox> SpeakerListContainer;
	void RefreshSpeakerList();
	void OnAddSpeaker();
	void OnRemoveSpeaker(int32 Index);
	void OnSpeakerComboChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo, int32 Index);
	FText GetSpeakerComboText(int32 Index) const;

	void UpdateSpeakerPreview();
	void UpdateDialoguePreview();

private:
	TArray<TSharedPtr<FString>> SpeakerOptions;
	TArray<TSharedPtr<FString>> DialogueOptions;

	// Cache for preview text to avoid redundant lookups
	FText CachedSpeakerPreview;
	FText CachedDialoguePreview;
	mutable FString LastPreviewLanguage;
};
