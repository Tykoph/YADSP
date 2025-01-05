#include "DialogueSystemEditor.h"
#include "AssetToolsModule.h"
#include "DialogueSystemAction.h"
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
        if (FName(TEXT("DialoguePin")) == Pin->PinType.PinSubCategory) {
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
void FYADSPEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    DialogueGraphAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DialogueGraph")), LOCTEXT("DialogueGraphAssetCategory", "Dialogue Graph"));
    TSharedPtr<FDialogueSystemAction> DialogueGraphAssetTypeAction = MakeShared<FDialogueSystemAction>(DialogueGraphAssetCategory);
    AssetTools.RegisterAssetTypeActions(DialogueGraphAssetTypeAction.ToSharedRef());

    DGStyleSet = MakeShareable(new FSlateStyleSet(TEXT("YADSPStyle")));
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("YADSP"));
    FString ContentDir = Plugin->GetBaseDir() / TEXT("Resources");
    DGStyleSet->SetContentRoot(ContentDir);

    FSlateImageBrush* ThumbnailBrush =      new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphThumbnailV2_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
    FSlateImageBrush* IconBrush =           new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
    FSlateImageBrush* NodeAddPinIcon =      new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
    FSlateImageBrush* NodeDeletePinIcon =   new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
    FSlateImageBrush* NodeDeleteNodeIcon =  new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon_128"), TEXT(".png")), FVector2D(128.0f, 128.0f));

    DGStyleSet->Set(TEXT("ClassThumbnail.DialogueSystem"), ThumbnailBrush);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueSystem"), IconBrush);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueSystem"), NodeAddPinIcon);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueSystem"), NodeDeletePinIcon);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueSystem"), NodeDeleteNodeIcon);

    FSlateStyleRegistry::RegisterSlateStyle(*DGStyleSet);

    PinFactory = MakeShareable(new FDialoguePinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);

    FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

void FYADSPEditorModule::ShutdownModule()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*DGStyleSet);
    FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FYADSPEditorModule, YADSPEditor)