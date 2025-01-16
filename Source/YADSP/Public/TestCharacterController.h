#pragma once

#include "CoreMinimal.h"
#include "TestCharacterController.Generated.h"

UCLASS()
class ATestCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	ATestCharacterController();
	virtual ~ATestCharacterController();

	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	class UDialogueSystem* DialogueSystemPtr = nullptr;

	UPROPERTY()
	class UDialoguePlayer* DialoguePlayerPtr = nullptr;

	UPROPERTY()
	class UNewQuestController* NewQuestControllerPtr = nullptr;
};
