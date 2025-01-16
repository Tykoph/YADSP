#include "UI/NewQuestController.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
// ---------- End Generated Includes Section ---------- //

UNewQuestController::UNewQuestController(const FObjectInitializer& objectInitializer) : UUserWidget(objectInitializer) {

}

// ---------- Generated Methods Section ---------- //
//             (Don't modify manually)             //
UNewQuestController* UNewQuestController::CreateInstance(APlayerController* PlayerController) {
    UNewQuestLoader* Loader = NewObject<UNewQuestLoader>(PlayerController);
    return Cast<UNewQuestController>(CreateWidget(PlayerController, Loader->WidgetTemplate));
}

UNewQuestLoader::UNewQuestLoader() {
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetTemplateFinder(*WidgetPath);
    WidgetTemplate = WidgetTemplateFinder.Class;
}
// ---------- End Generated Methods Section ---------- //
