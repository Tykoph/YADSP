// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "CoreMinimal.h"
#include "DialoguePlayer.h"
#include "Components/BillboardComponent.h"
#include "DialogueActor.Generated.h"

class UDialoguePlayer;

UCLASS(Blueprintable)
class YADSP_API ADialogueActor : public AActor
{
	GENERATED_BODY()

	ADialogueActor();
	virtual ~ADialogueActor() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UPROPERTY(EditAnywhere)
	UDialogueSystem* DialogueSystem = nullptr;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<AActor*> TalkingActors;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<ADialogueCamera*> CameraActors;

	UFUNCTION(BlueprintCallable)
	void PlayDialogue(APlayerController* PlayerController, FDialogueEndCallback OnDialogueEnded) const;

protected:
#if WITH_EDITORONLY_DATA
	/** Billboard component displayed in the editor. */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UBillboardComponent> SpriteComponent;
#endif

	/** Get the editor only billboard component of the actor. */
	UFUNCTION(BlueprintPure, Category = "Components", meta = (DevelopmentOnly))
	UBillboardComponent* GetEditorSpriteComponent() const;

private:
	UFUNCTION()
	void UpdateArrayLenght();

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDialoguePlayer> DialoguePlayer;
};
