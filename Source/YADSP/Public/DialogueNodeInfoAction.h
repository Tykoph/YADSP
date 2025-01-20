#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueAction.h"
#include "DialogueNodeInfoAction.Generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoAction : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EDialogueAction Action = EDialogueAction::None;

	UPROPERTY(EditAnywhere)
	FString ActionData = TEXT("");
};
