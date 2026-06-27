// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueSystem.h"
#include "CoreMinimal.h"
#include "DialoguePlayer.h"
#include "Components/BillboardComponent.h"
#include "DialogueActor.generated.h"

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

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDialogueSystem> DialogueSystem = nullptr;

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
	void PlayDialogue(APlayerController* PlayerController, const FDialogueEndCallback& OnDialogueEnded) const;

protected:
	
	// Get the editor only billboard component of the actor.
	UFUNCTION(BlueprintPure, Category = "Components", meta = (DevelopmentOnly))
	UBillboardComponent* GetEditorSpriteComponent() const;
	
#if WITH_EDITORONLY_DATA
	// Billboard component displayed in the editor.
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UBillboardComponent> SpriteComponent;
#endif

private:
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDialoguePlayer> DialoguePlayer;
};
