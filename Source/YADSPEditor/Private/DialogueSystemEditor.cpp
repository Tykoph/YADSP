// Copyright Tom Duby. All Rights Reserved.

#include "YADSPEditor/Public/DialogueSystemEditor.h"

#include "YADSPEditor/Public/DialogueSystemAction.h"
#include "Factory/DialogueGraphFactories.h"

#include "IAssetTools.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FYADSPEditorModule"

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
	const TSharedPtr<FDialogueSystemAction> DialogueGraphAssetTypeAction = MakeShared<FDialogueSystemAction>(AssetType);
	AssetTools.RegisterAssetTypeActions(DialogueGraphAssetTypeAction.ToSharedRef());

	DGStyleSet = MakeShareable(new FSlateStyleSet(TEXT("YADSPStyle")));
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("YADSP"));
	const FString ContentDir = Plugin->GetBaseDir() / TEXT("Resources");
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
