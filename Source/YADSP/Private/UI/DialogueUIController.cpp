#include "UI/DialogueUIController.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
// ---------- End Generated Includes Section ---------- //

UDialogueUIController::UDialogueUIController(const FObjectInitializer& objectInitializer) : UUserWidget(objectInitializer) {

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
