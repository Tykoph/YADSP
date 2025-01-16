#include "TestCharacterController.h"
#include "DialogueSystem.h"
#include "DialogueNodeInfoEnd.h"
#include "Components/TextBlock.h"
#include "UI/DialoguePlayer.h"
#include "UI/NewQuestController.h"

DEFINE_LOG_CATEGORY_STATIC(ATestCharacterControllerSub, Log, All);

ATestCharacterController::ATestCharacterController()
{
	static ConstructorHelpers::FObjectFinder<UDialogueSystem> DialogueSystemFinder(TEXT("/YADSP/DG_ExampleDialogueGraph"));
	if (DialogueSystemFinder.Succeeded()) {
		DialogueSystemPtr = DialogueSystemFinder.Object;
	} else {
		UE_LOG(ATestCharacterControllerSub, Error, TEXT("Failed to find dialogue system asset"));
	}
}

ATestCharacterController::~ATestCharacterController()
{
}

void ATestCharacterController::BeginPlay()
{
	Super::BeginPlay();

	// Play the dialogue
	DialoguePlayerPtr = NewObject<UDialoguePlayer>(this);
	DialoguePlayerPtr->PlayDialogue(DialogueSystemPtr, this, [this](EDialogueNodeAction Action, FString Data) {
		if (NewQuestControllerPtr != nullptr)
		{
			NewQuestControllerPtr->RemoveFromParent();
		}
		NewQuestControllerPtr = UNewQuestController::CreateInstance(this);
		NewQuestControllerPtr->QuestText->SetText(FText::FromString(Data));
		NewQuestControllerPtr->AddToViewport();
	});

}
