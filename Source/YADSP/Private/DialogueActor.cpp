﻿#include "DialogueActor.h"
#include "DialoguePlayer.h"

// Constructor: Initializes the dialogue actor, sets up editor sprite component and registers blueprint compilation callback
ADialogueActor::ADialogueActor()
{
	if (GEditor) {
		GEditor->OnBlueprintCompiled().AddUObject(this, &ADialogueActor::UpdateArrayLenght);
	}

	DialoguePlayer = CreateDefaultSubobject<UDialoguePlayer>("DialoguePlayer");

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	RootComponent = SpriteComponent;

	if (!IsRunningCommandlet() && (SpriteComponent != nullptr)) {
		// Structure to hold one-time initialization.
		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture;
			FName DialogueActorId;
			FText DialogueActorName;

			FConstructorStatics():
				SpriteTexture(TEXT("/YADSP/DialogueActorIcon")),
				DialogueActorId(TEXT("DialogueActor")),
				DialogueActorName(NSLOCTEXT("SpriteCategory", "DialogueActor", "Dialogue Actors"))
			{}
		};
		static FConstructorStatics ConstructorStatics;

		SpriteComponent->Sprite = ConstructorStatics.SpriteTexture.Get();
		SpriteComponent->SpriteInfo.Category = ConstructorStatics.DialogueActorId;
		SpriteComponent->SpriteInfo.DisplayName = ConstructorStatics.DialogueActorName;
		SpriteComponent->bIsScreenSizeScaled = true;
	}
#endif
}

// Destructor: Unregisters from blueprint compilation events to prevent callbacks after destruction
ADialogueActor::~ADialogueActor()
{
	if (GEditor) {
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
	TalkingActors.SetNum(DialogueSystem->SpeakerStringArray.Num());
}

// Returns the editor-only billboard component used for visualization in the editor
UBillboardComponent* ADialogueActor::GetEditorSpriteComponent() const
{
#if WITH_EDITORONLY_DATA
	return SpriteComponent.Get();
#else
	return nullptr;
#endif
}
