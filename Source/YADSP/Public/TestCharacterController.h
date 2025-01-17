#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoEnd.h"
#include "UI/DialoguePlayer.h"
#include "TestCharacterController.Generated.h"

UCLASS()
class ATestCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	ATestCharacterController();
	virtual ~ATestCharacterController() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDialogueEnded(EDialogueNodeAction Action, FString Data);

protected:
	UPROPERTY()
	class UDialogueSystem* DialogueSystemPtr = nullptr;

	UPROPERTY()
	class UDialoguePlayer* DialoguePlayerPtr = nullptr;

	UPROPERTY()
	class UNewQuestController* NewQuestControllerPtr = nullptr;
};
