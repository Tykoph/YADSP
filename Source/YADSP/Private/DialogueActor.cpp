// Copyright Tom Duby. All Rights Reserved.

#include "DialogueActor.h"
#include "DialoguePlayer.h"

/**
 * Initializes the dialogue actor, sets up the editor sprite component, and registers the blueprint compilation callback.
 */
ADialogueActor::ADialogueActor()
{
	DialoguePlayer = CreateDefaultSubobject<UDialoguePlayer>("DialoguePlayer");

// Editor-only properties for visualization and blueprint compilation events
#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	RootComponent = SpriteComponent;

	if (!IsRunningCommandlet() && (SpriteComponent != nullptr)) {
		// Holds one-time initialization data for the dialogue actor's editor sprite.
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

ADialogueActor::~ADialogueActor()
{
// Cleanup editor-only event bindings
#if WITH_EDITOR
	if (GEditor) {
		GEditor->OnBlueprintCompiled().RemoveAll(this);
	}
#endif
}

void ADialogueActor::PlayDialogue(APlayerController* InPlayerController, const FDialogueEndCallback OnDialogueEnded) const
{
	DialoguePlayer->PlayDialogue(DialogueSystem, InPlayerController, OnDialogueEnded);
}

UBillboardComponent* ADialogueActor::GetEditorSpriteComponent() const
{
#if WITH_EDITORONLY_DATA
	return SpriteComponent.Get();
#else
	return nullptr;
#endif
}
