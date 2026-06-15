// Copyright 2026 Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueSkipEnum.h"
#include "DialogueNodeInfoText.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPropertiesChanged);

UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditAnywhere)
	FString Title;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetSpeakerFromTable"))
	TArray<FName> SpeakerIDs;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetDialogueFromTable"))
	FName DialogueID;
	
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetDialogueFromTable"))
	TArray<FName> DialogueResponses;
	
	UPROPERTY(EditAnywhere)
	USoundCue* DialogueSound;

	UPROPERTY(EditAnywhere)
	ESkipDialogue SkipDialogue = ESkipDialogue::NoSkip;

	UPROPERTY(EditAnywhere, meta=(EditCondition="SkipDialogue == ESkipDialogue::AutoSkipAfterTime", ClampMin = 0))
	float SkipAfterSeconds;

	FOnPropertiesChanged OnPropertiesChanged;
	
public:
	UFUNCTION()
	TArray<FString> GetSpeakerFromTable() const;

	UFUNCTION()
	FString GetSpeakerName(const FName& SpeakerName) const;

	UFUNCTION()
	TArray<FString> GetDialogueFromTable() const;
	
	UFUNCTION()
	FString GetDialogueText(const FName& DialogueText) const;
	
	UFUNCTION()
	TArray<FString> GetDialogueOptionText(const TArray<FName>& DialogueOptionText) const;
};
