#include "DialogueActor.h"
#include "DialoguePlayer.h"

ADialogueActor::ADialogueActor()
{
	if (GEditor)
	{
		GEditor->OnBlueprintCompiled().AddUObject(this, &ADialogueActor::UpdateArrayLenght);
	}

	DialoguePlayer = CreateDefaultSubobject<UDialoguePlayer>("DialoguePlayer");
}

ADialogueActor::~ADialogueActor()
{
	if (GEditor)
	{
		GEditor->OnBlueprintCompiled().RemoveAll(this);
	}
}

void ADialogueActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateArrayLenght();
}

void ADialogueActor::PlayDialogue(APlayerController* PlayerController, FDialogueEndCallback OnDialogueEnded) const
{
	DialoguePlayer->PlayDialogue(DialogueSystem, PlayerController, CameraActors, OnDialogueEnded);
}

void ADialogueActor::UpdateArrayLenght()
{
	UE_LOG(LogTemp, Log, TEXT("UpdateArrayLenght"));
	if (DialogueSystem == nullptr) { return; }
	CameraActors.SetNum(DialogueSystem->CameraStringArray.Num());
}
