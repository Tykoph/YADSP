#include "UI/DialogueUIController.h"
#include "Fonts/FontMeasure.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
// ---------- End Generated Includes Section ---------- //

UDialogueUIController::UDialogueUIController(const FObjectInitializer& objectInitializer) : UUserWidget(objectInitializer) {

}

void UDialogueUIController::IsTextWrapping(UTextBlock* InDialogueText, const FString& Text)
{
    const FSlateFontInfo FontInfo = InDialogueText->GetFont();
    const TSharedPtr<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

    // Calculate Text Width
    if (FontMeasure.IsValid())
    {
        const FVector2D TextSize = FontMeasure->Measure(Text, FontInfo);
        const FVector2D ContainerText = InDialogueText->GetCachedGeometry().GetLocalSize();
        if (TextSize.X > ContainerText.X)
        {
            InDialogueText->SetJustification(ETextJustify::Left);
        }
        else
        {
            InDialogueText->SetJustification(ETextJustify::Center);
        }
    }
}

// ---------- Generated Methods Section ---------- //
//             (Don't modify manually)             //
UDialogueUIController* UDialogueUIController::CreateInstance(APlayerController* PlayerController) {
    UDialogueUILoader* Loader = NewObject<UDialogueUILoader>(PlayerController);
    return Cast<UDialogueUIController>(CreateWidget(PlayerController, Loader->WidgetTemplate));
}

UDialogueUILoader::UDialogueUILoader() {
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetTemplateFinder(*WidgetPath);
    WidgetTemplate = WidgetTemplateFinder.Class;
}
// ---------- End Generated Methods Section ---------- //
