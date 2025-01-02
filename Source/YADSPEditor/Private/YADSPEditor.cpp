#include "YADSPEditor.h"
#include "AssetToolsModule.h"
#include "AssetTypeActions_DialogueGraph.h"
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
        return FLinearColor::Blue;
    }
};

struct FDialoguePinFactory : public FGraphPanelPinFactory
{
public:
    virtual ~FDialoguePinFactory() {}
    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
    {
        if (FName(TEXT("DialoguePin")) == Pin->PinType.PinSubCategory)
        {
            return SNew(SDialogueGraphPin, Pin);
        }

        return nullptr;
    }
};

// Create the DialogueGraph asset category and register the DialogueGraph asset in the context menu
void FYADSPEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    DialogueGraphAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("DialogueGraph")), LOCTEXT("DialogueGraphAssetCategory", "Dialogue Graph"));
    TSharedPtr<FAssetTypeActions_DialogueGraph> DialogueGraphAssetTypeAction = MakeShared<FAssetTypeActions_DialogueGraph>(DialogueGraphAssetCategory);
    AssetTools.RegisterAssetTypeActions(DialogueGraphAssetTypeAction.ToSharedRef());

    DGStyleSet = MakeShareable(new FSlateStyleSet(TEXT("YADSPStyle")));
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("YADSP"));
    FString ContentDir = Plugin->GetContentDir();
    DGStyleSet->SetContentRoot(ContentDir);

    FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphThumbnail"), TEXT(".png")), FVector2D(512.0f, 512.0f));
    FSlateImageBrush* IconBrush = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon"), TEXT(".png")), FVector2D(512.0f, 512.0f));
    FSlateImageBrush* NodeAddPinIcon = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon"), TEXT(".png")), FVector2D(512.0f, 512.0f));
    FSlateImageBrush* NodeDeletePinIcon = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon"), TEXT(".png")), FVector2D(512.0f, 512.0f));
    FSlateImageBrush* NodeDeleteNodeIcon = new FSlateImageBrush(DGStyleSet->RootToContentDir(TEXT("DialogueGraphIcon"), TEXT(".png")), FVector2D(512.0f, 512.0f));

    DGStyleSet->Set(TEXT("ClassThumbnail.DialogueGraph"), ThumbnailBrush);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueGraph"), IconBrush);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueGraph"), NodeAddPinIcon);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueGraph"), NodeDeletePinIcon);
    DGStyleSet->Set(TEXT("ClassIcon.DialogueGraph"), NodeDeleteNodeIcon);

    FSlateStyleRegistry::RegisterSlateStyle(*DGStyleSet);

    PinFactory = MakeShareable(new FDialoguePinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FYADSPEditorModule::ShutdownModule()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*DGStyleSet);
    FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FYADSPEditorModule, YADSPEditor)