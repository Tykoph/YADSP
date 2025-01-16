#include "UI/DialogueOptionController.h"

// ---------- Generated Includes Section ---------- //
//             (Don't modify manually)              //
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/NewQuestController.h"
// ---------- End Generated Includes Section ---------- //

UDialogueOptionController::UDialogueOptionController(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {

}

void UDialogueOptionController::SetClickHandler(int Index, std::function<void(int)> ClickHandler)
{
    ButtonIndex = Index;
    OnClickHandler = ClickHandler;

    ResponseButton->OnClicked.AddUniqueDynamic(this, &UDialogueOptionController::OnButtonClicked);
}

void UDialogueOptionController::OnButtonClicked()
{
    if(OnClickHandler)
    {
        OnClickHandler(ButtonIndex);
    }
}

// ---------- Generated Methods Section ---------- //
//             (Don't modify manually)             //
UDialogueOptionController* UDialogueOptionController::CreateInstance(APlayerController* PlayerController) {
    UDialogueOptionLoader* Loader = NewObject<UDialogueOptionLoader>(PlayerController);
    return Cast<UDialogueOptionController>(CreateWidget(PlayerController, Loader->WidgetTemplate));
}

UDialogueOptionLoader::UDialogueOptionLoader() {
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetTemplateFinder(*WidgetPath);
    WidgetTemplate = WidgetTemplateFinder.Class;
}
// ---------- End Generated Methods Section ---------- //