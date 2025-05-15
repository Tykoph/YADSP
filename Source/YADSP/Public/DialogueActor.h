// Copyright 2025 Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "CoreMinimal.h"
#include "DialoguePlayer.h"
#include "Components/BillboardComponent.h"
#include "DialogueActor.Generated.h"

class UDialoguePlayer;

/**
 * @brief Actor class responsible for managing and playing dialogue sequences in the game.
 *
 * The DialogueActor serves as a placement point for dialogue interactions in the game world.
 * It manages references to:
 * - The DialogueSystem that defines the conversation structure
 * - The actors participating in the dialogue
 * - Camera actors used for different dialogue shots
 * 
 * In the editor, it displays a billboard sprite for easy identification and placement.
 * The actor automatically maintains array sizes based on the DialogueSystem configuration.
 */
UCLASS(Blueprintable)
class YADSP_API ADialogueActor : public AActor
{
	GENERATED_BODY()

	ADialogueActor();
	virtual ~ADialogueActor() override;

	/**
	 * @brief Called when a property is changed in the editor.
	 *
	 * This method is triggered whenever a property of the DialogueActor is modified in the editor.
	 * It ensures that the arrays for talking actors and cameras are properly sized according to
	 * the current DialogueSystem configuration.
	 *
	 * @param PropertyChangedEvent Information about which property was changed
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UPROPERTY(EditAnywhere)
	UDialogueSystem* DialogueSystem = nullptr;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<AActor*> TalkingActors;

	UPROPERTY(EditAnywhere, EditFixedSize)
	TArray<ADialogueCamera*> CameraActors;

	/**
	 * @brief Initiates playback of the dialogue sequence.
	 *
	 * Starts the dialogue playback using the configured DialogueSystem. This method handles:
	 * - Setting up the dialogue player with the current dialogue system
	 * - Configuring camera actors for dialogue shots
	 * - Managing the player controller during dialogue
	 * - Executing callback when dialogue completes
	 *
	 * @param PlayerController The player controller that will be used during dialogue playback
	 * @param OnDialogueEnded Callback function that will be executed when the dialogue sequence completes
	 */
	UFUNCTION(BlueprintCallable)
	void PlayDialogue(APlayerController* PlayerController, FDialogueEndCallback OnDialogueEnded) const;

protected:
#if WITH_EDITORONLY_DATA
	// Billboard component displayed in the editor.
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UBillboardComponent> SpriteComponent;
#endif

	// Get the editor only billboard component of the actor.
	UFUNCTION(BlueprintPure, Category = "Components", meta = (DevelopmentOnly))
	UBillboardComponent* GetEditorSpriteComponent() const;

private:
	/**
	 * @brief Updates the length of TalkingActors and CameraActors arrays.
	 *
	 * This method ensures that the arrays for talking actors and cameras match
	 * the size of corresponding arrays in the DialogueSystem. It is called:
	 * - When properties are changed in the editor
	 * - When blueprints are compiled.
	 * 
	 * If DialogueSystem is null, no updates are performed.
	 */
	UFUNCTION()
	void UpdateArrayLenght();

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDialoguePlayer> DialoguePlayer;
};
