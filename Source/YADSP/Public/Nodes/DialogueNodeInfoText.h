// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueProgressEnum.h"
#include "DialogueNodeInfoText.generated.h"

/**
 * Node info class storing speakers and dialogue keys.
 */
UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoText : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/**
	 * Fetches valid speaker IDs from the referenced data table.
	 * @return Array of valid speaker IDs.
	 */
	UFUNCTION()
	TArray<FString> GetSpeakerFromTable() const;

	/**
	 * Fetches valid dialogue text IDs from the referenced data table.
	 * @return Array of valid dialogue text IDs.
	 */
	UFUNCTION()
	TArray<FString> GetDialogueFromTable() const;
	
	// The internal title or description for this dialogue node.
	UPROPERTY(EditAnywhere, Category="YADSP")
	FString Title;
	
	// Localization keys for the speakers associated with this text.
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetSpeakerFromTable"), Category="YADSP")
	TArray<FName> SpeakerKeys;

	// Localization key pointing to the dialogue text content.
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetDialogueFromTable"), Category="YADSP")
	FName DialogueKey;

	// TODO: rework the skip system to be dialogue or project managed. Maybe keep an override parameter on text nodes. 
	// Determines if and how this text automatically progresses.
	UPROPERTY(EditAnywhere, Category="YADSP")
	EDialogueProgression ProgressDialogue = EDialogueProgression::WaitForInput;
	// Custom time in seconds to wait before auto-skipping.
	UPROPERTY(EditAnywhere, meta=(EditCondition="ProgressDialogue == EDialogueProgression::AutoSkipAfterTime", ClampMin = 0), Category="YADSP")
	float ProgressAfterSeconds;
};
