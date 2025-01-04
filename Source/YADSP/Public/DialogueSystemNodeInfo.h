#pragma once

#include "CoreMinimal.h"
#include "DialogueSystemNodeInfo.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueSystemNodeInfo : public UObject
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
