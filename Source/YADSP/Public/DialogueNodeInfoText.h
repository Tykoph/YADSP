#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfoText.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	FText DialogueText;

	UPROPERTY(EditAnywhere)
	TArray<FText> DialogueResponses;
};
