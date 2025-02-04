#include "DialogueActor.h"

ADialogueActor::ADialogueActor()
{
	if (GEditor)
	{
		GEditor->OnBlueprintCompiled().AddUObject(this, &ADialogueActor::UpdateArrayLenght);
	}
}

ADialogueActor::~ADialogueActor()
{
	if (GEditor)
	{
		GEditor->OnBlueprintCompiled().RemoveAll(this);
	}
}

void ADialogueActor::UpdateArrayLenght()
{
	UE_LOG(LogTemp, Log, TEXT("UpdateArrayLenght"));
	if (DialogueSystem == nullptr) { return; }
	CameraActors.SetNum(DialogueSystem->CameraStringArray.Num());
}
