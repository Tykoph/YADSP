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
	FText Speaker;

	UPROPERTY(EditAnywhere)
	FText DialogueText;

	UPROPERTY(EditAnywhere, meta = (ClampMin = -1))
	int32 CameraIndex = -1;

	UPROPERTY(EditAnywhere)
	USoundCue* DialogueSound;

	/** Work only if there is one answer.
	 * -1 will auto-determine skip time base on text length,
	 * 0 will use the Sound Cue length or disable auto-skip if there is no Cue */
	UPROPERTY(EditAnywhere, meta = (ClampMin = -1))
	float SkipAfterSeconds;

	UPROPERTY(EditAnywhere)
	TArray<FText> DialogueResponses;

};
