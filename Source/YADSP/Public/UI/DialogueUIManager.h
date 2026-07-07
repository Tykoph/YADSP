// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSubsystem.h"
#include "DialogueWidget.h"
#include "DialogueUIManager.generated.h"

/**
 * Component managing the top-level creation, display, and cleanup of the dialogue interface.
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class YADSP_API UDialogueUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/**
	 * Handler for standard dialogue start, allocating UI onto the viewport.
	 * @param InDialogueAsset The dialogue being processed.
	 * @param InPlayerController The owning player controller.
	 */
	UFUNCTION()
	void OnDialogueStarted(UDialogueSystem* InDialogueAsset, APlayerController* InPlayerController);
	
	/**
	 * Disposes UI elements upon dialogue termination.
	 */
	UFUNCTION()
	void OnDialogueEnded();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="YADSP")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	TObjectPtr<UUserWidget> DialogueUI;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;	
};
