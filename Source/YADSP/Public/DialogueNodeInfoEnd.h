#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfoEnd.generated.h"

UENUM()
enum class EDialogueNodeAction : uint8
{
	None,
	StartQuest,
};


UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoEnd : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EDialogueNodeAction Action = EDialogueNodeAction::None;

	UPROPERTY(EditAnywhere)
	FString ActionData = TEXT("");
};
