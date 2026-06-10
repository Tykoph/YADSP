// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once
#include "DialogueSubsystem.h"
#include "DialogueUIController.h"
#include "DialogueUIManager.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class YADSP_API UDialogueUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnDialogueStarted(UDialogueSystem* DialogueAsset, APlayerController* PC);
	
	UFUNCTION()
	void OnDialogueEnded();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDialogueUIController> DialogueUIClass;
	
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* DialogueUI;
	
private:
	UPROPERTY()
	UDialogueSubsystem* DialogueSubsystem;	
};
