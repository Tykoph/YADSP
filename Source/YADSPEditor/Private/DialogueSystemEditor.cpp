// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueSystemEditor.h"

#include "DialogueActor.h"
#include "DialogueSystemAction.h"
#include "IAssetTools.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FYADSPEditorModule"

/**
 * Custom graph pin for dialogue text connections
 * Displays with blue color
 */
class SDialogueGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphPin)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

private:
	virtual FSlateColor GetPinColor() const override
	{
		return FColor::Blue;
	}
};

/**
 * Custom graph pin for dialogue start node connections
 * Displays with red color
 */
class SDialogueGraphStartPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphPin)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

private:
	virtual FSlateColor GetPinColor() const override
	{
		return FColor::Red;
	}
};

/**
 * Custom graph pin for dialogue end node connections
 * Displays with purple color
 */
class SDialogueGraphEndPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphPin)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

private:
	virtual FSlateColor GetPinColor() const override
	{
		return FColor::Purple;
	}
};

/**
 * Custom graph pin for dialogue action node connections
 * Displays with green color
 */
class SDialogueGraphActionPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphPin)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

private:
	virtual FSlateColor GetPinColor() const override
	{
		return FColor::Green;
	}
};

/**
 * Custom graph pin for dialogue animation node connections
 * Displays with yellow color
 */
class SDialogueGraphAnimationPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphPin)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	}

private:
	virtual FSlateColor GetPinColor() const override
	{
		return FColor::Yellow;
	}
};

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
}

/**
 * Cleans up the Dialogue System editor module
 * Unregisters styles and pin factory
 */
void FDialogueSystemEditorModule::ShutdownModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*DGStyleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDialogueSystemEditorModule, YADSPEditor)
