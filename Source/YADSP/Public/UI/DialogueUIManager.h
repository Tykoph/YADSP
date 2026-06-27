// Copyright Tom Duby. All Rights Reserved.

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnDialogueStarted(UDialogueSystem* DialogueAsset, APlayerController* PC);
	
	UFUNCTION()
	void OnDialogueEnded();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDialogueUIController> DialogueUIClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUserWidget> DialogueUI;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;	
};
