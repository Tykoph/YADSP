#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "DialogueNodeInfoEnd.h"
#include "DialogueSystemRuntimeGraph.h"
#include "DialoguePlayer.generated.h"

UCLASS()
class YADSP_API UDialoguePlayer : public UObject
{
	GENERATED_BODY()

public:
	void PlayDialogue(class	UDialogueSystem* DialogueAsset, APlayerController* PlayerController, std::function<void(EDialogueNodeAction Action, FString Data)> OnDialogueEnded);
	void ChooseOptionAtIndex(int Index);

private:
	UPROPERTY()
	class UDialogueSystem* DialogueAssetPtr = nullptr;

	UPROPERTY()
	class UDialogueRuntimeGraphNode* CurrentNodePtr = nullptr;

	UPROPERTY()
	class UDialogueUIController* DialogueUIPtr = nullptr;

	std::function<void(EDialogueNodeAction Action, FString Data)> OnDialogueEndedCallback;
};
