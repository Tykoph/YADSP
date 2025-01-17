#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoEnd.h"
#include "DialogueSystemRuntimeGraph.h"
#include "DialoguePlayer.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDialogueEndCallback, EDialogueNodeAction, Action, FString, Data);

UCLASS(ClassGroup = (DialogueSystem), meta = (BlueprintSpawnableComponent))
class YADSP_API UDialoguePlayer : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Play Dialogue"), Category = "Dialogue System")
	void PlayDialogue(class	UDialogueSystem* DialogueAsset, APlayerController* PlayerController, TArray<AActor*> Cameras,FDialogueEndCallback OnDialogueEnded);

	void ChooseOptionAtIndex(int Index);
	float CalculateSkipTimer(const FText& Text);
	void AutoSkipDialogue(float Time);

private:
	UPROPERTY()
	class UDialogueSystem* DialogueAssetPtr = nullptr;

	UPROPERTY()
	class UDialogueRuntimeGraphNode* CurrentNodePtr = nullptr;

	UPROPERTY()
	class UDialogueUIController* DialogueUIPtr = nullptr;

	UPROPERTY()
	class APlayerController* PlayerControllerPtr = nullptr;

	float CurrentSkipTime = 0.0f;

	FDialogueEndCallback OnDialogueEndedCallback;
};
