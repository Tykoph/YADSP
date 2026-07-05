// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueSkipEnum.h"
#include "DialogueNodeInfoText.generated.h"

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditAnywhere, Category="YADSP")
	FString Title;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetSpeakerFromTable"), Category="YADSP")
	TArray<FName> SpeakerKeys;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetDialogueFromTable"), Category="YADSP")
	FName DialogueKey;

	UPROPERTY(EditAnywhere, Category="YADSP")
	ESkipDialogue SkipDialogue = ESkipDialogue::NoSkip;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SkipDialogue == ESkipDialogue::AutoSkipAfterTime", ClampMin = 0), Category="YADSP")
	float SkipAfterSeconds;
	
public:
	UFUNCTION()
	TArray<FString> GetSpeakerFromTable() const;

	UFUNCTION()
	TArray<FString> GetDialogueFromTable() const;
};
