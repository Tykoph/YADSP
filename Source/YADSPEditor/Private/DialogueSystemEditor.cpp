#include "DialogueSystemEditor.h"
#include "DialogueSystemAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "DialogueActorAction.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FYADSPEditorModule"

class SDialogueGraphPin : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SDialogueGraphPin) {}
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

class SDialogueGraphStartPin : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SDialogueGraphPin) {}
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

class SDialogueGraphEndPin : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SDialogueGraphPin) {}
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

struct FDialoguePinFactory : public FGraphPanelPinFactory
{
public:
    virtual ~FDialoguePinFactory() {}
    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
    {
        if (FName(TEXT("TextPin")) == Pin->PinType.PinSubCategory) {
            return SNew(SDialogueGraphPin, Pin);
        } else if (FName(TEXT("StartPin")) == Pin->PinType.PinSubCategory) {
            return SNew(SDialogueGraphStartPin, Pin);
        } else if (FName(TEXT("EndPin")) == Pin->PinType.PinSubCategory) {
            return SNew(SDialogueGraphEndPin, Pin);
        }

        return nullptr;
    }
};

// Create the DialogueGraph asset category and register the DialogueGraph asset in the context menu
void FDialogueSystemEditorModule::StartupModule()
{
    IAssetTools& AssetTools = IAssetTools::Get();

    EAssetTypeCategories::Type AssetType = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DialogueGraph")), LOCTEXT("DialogueGraphAssetCategory", "Dialogue Graph"));
    TSharedPtr<DialogueSystemAction> DialogueGraphAssetTypeAction = MakeShared<DialogueSystemAction>(AssetType);
    AssetTools.RegisterAssetTypeActions(DialogueGraphAssetTypeAction.ToSharedRef());

    EAssetTypeCategories::Type  ActorType = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DialogueActor")), LOCTEXT("DialogueActorAssetCategory", "Dialogue Actor"));
    TSharedPtr<DialogueActorAction> DialogueActorAssetTypeAction = MakeShared<DialogueActorAction>(ActorType);
    AssetTools.RegisterAssetTypeActions(DialogueActorAssetTypeAction.ToSharedRef());

    DGStyleSet = MakeShareable(new FSlateStyleSet(TEXT("YADSPStyle")));
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("YADSP"));
    FString ContentDir = Plugin->GetBaseDir() / TEXT("Resources");
    DGStyleSet->SetContentRoot(ContentDir);

    FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphThumbnailV2_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
    FSlateImageBrush* IconBrush = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));

    DGStyleSet->Set(TEXT("ClassThumbnail.DialogueSystem"), ThumbnailBrush);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueSystem"), IconBrush);

    FSlateStyleRegistry::RegisterSlateStyle(*DGStyleSet);

    PinFactory = MakeShareable(new FDialoguePinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FDialogueSystemEditorModule::ShutdownModule()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*DGStyleSet);
    FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDialogueSystemEditorModule, YADSPEditor)