// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSubsystem.h"
#include "DialogueWidget.h"
#include "DialogueUIManager.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class YADSP_API UDialogueUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void OnDialogueStarted(UDialogueSystem* InDialogueAsset, APlayerController* InPlayerController);
	
	UFUNCTION()
	void OnDialogueEnded();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UUserWidget> DialogueUI;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;	
};
