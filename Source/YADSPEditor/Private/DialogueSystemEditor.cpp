// Copyright 2026 Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/DialogueSystemEditor.h"

#include "YADSPEditor/Public/DialogueSystemAction.h"
#include "YADSPEditor/Public/Nodes/DialogueGraphNodeText.h"
#include "IAssetTools.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphPin.h"
#include "YADSPEditor/Public/UI/SDialogueGraphNodeText.h"
#include "YADSPEditor/Public/UI/SDialogueGraphPins.h"

#define LOCTEXT_NAMESPACE "FYADSPEditorModule"

struct FDialoguePinFactory : FGraphPanelPinFactory
{
	virtual ~FDialoguePinFactory() override
	{
	}

	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if (FName(TEXT("TextPin")) == Pin->PinType.PinSubCategory) {
			return SNew(SDialogueGraphPin, Pin);
		}
		if (FName(TEXT("StartPin")) == Pin->PinType.PinSubCategory) {
			return SNew(SDialogueGraphStartPin, Pin);
		}
		if (FName(TEXT("EndPin")) == Pin->PinType.PinSubCategory) {
			return SNew(SDialogueGraphEndPin, Pin);
		}
		if (FName(TEXT("ActionPin")) == Pin->PinType.PinSubCategory) {
			return SNew(SDialogueGraphActionPin, Pin);
		}
		if (FName(TEXT("AnimationPin")) == Pin->PinType.PinSubCategory) {
			return SNew(SDialogueGraphAnimationPin, Pin);
		}

		return nullptr;
	}
};

struct FDialogueGraphNodeFactory : FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UDialogueGraphNodeText* TextNode = Cast<UDialogueGraphNodeText>(Node)) {
			return SNew(SDialogueGraphNodeText, TextNode);
		}
		return nullptr;
	}
};

/**
 * Initializes the Dialogue System editor module
 * Registers DialogueGraph asset category in the content browser
 * Creates and registers custom style set for icons and thumbnails
 * Registers custom pin factory for different dialogue graph node connections
 */
void FDialogueSystemEditorModule::StartupModule()
{
	IAssetTools& AssetTools = IAssetTools::Get();

	EAssetTypeCategories::Type AssetType = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("DialogueGraph")),
		LOCTEXT("DialogueGraphAssetCategory", "Dialogue Graph"));
	TSharedPtr<DialogueSystemAction> DialogueGraphAssetTypeAction = MakeShared<DialogueSystemAction>(AssetType);
	AssetTools.RegisterAssetTypeActions(DialogueGraphAssetTypeAction.ToSharedRef());

	DGStyleSet = MakeShareable(new FSlateStyleSet(TEXT("YADSPStyle")));
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("YADSP"));
	FString ContentDir = Plugin->GetBaseDir() / TEXT("Resources");
	DGStyleSet->SetContentRoot(ContentDir);

	FSlateImageBrush* ThumbnailGraphBrush = new FSlateImageBrush(
		DGStyleSet->RootToContentDir(
			TEXT("DialogueGraphThumbnail_128"),
			TEXT(".png")),
		FVector2D(128.0f, 128.0f));
	FSlateImageBrush* ThumbnailActorBrush = new FSlateImageBrush(
		DGStyleSet->RootToContentDir(
			TEXT("DialogueActorThumbnail"),
			TEXT(".png")),
		FVector2D(128.0f, 128.0f));
	FSlateImageBrush* IconBrush = new FSlateImageBrush(
		DGStyleSet->RootToContentDir(
			TEXT("DialogueGraphIcon_128"),
			TEXT(".png")),
		FVector2D(128.0f, 128.0f));

	DGStyleSet->Set(TEXT("ClassThumbnail.DialogueSystem"), ThumbnailGraphBrush);
	DGStyleSet->Set(TEXT("ClassIcon.DialogueSystem"), IconBrush);
	DGStyleSet->Set(TEXT("ClassThumbnail.DialogueActor"), ThumbnailActorBrush);

	FSlateStyleRegistry::RegisterSlateStyle(*DGStyleSet);

	// Create and register custom pin factory for dialogue graph pins
	PinFactory = MakeShareable(new FDialoguePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);

	// Create and register custom node factory for dialogue graph nodes
	NodeFactory = MakeShareable(new FDialogueGraphNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);
}

/**
 * Cleans up the Dialogue System editor module
 * Unregisters styles and pin factory
 */
void FDialogueSystemEditorModule::ShutdownModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*DGStyleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
	FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDialogueSystemEditorModule, YADSPEditor)
