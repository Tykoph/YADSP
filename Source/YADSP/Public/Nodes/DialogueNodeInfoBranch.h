// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueNodeInfoBase.h"
#include "BranchCondition.h"
#include "DialogueNodeInfoBranch.generated.h"

/**
 * Node info class defining a branching point in the dialogue flow.
 */
UCLASS()
class YADSP_API UDialogueNodeInfoBranch : public UDialogueNodeInfoBase
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/**
	 * Retrieves the list of available option texts.
	 * @return Array of localized text keys.
	 */
	UFUNCTION()
	TArray<FString> GetTextList() const;
	
	/**
	 * Retrieves the list of available option tooltips.
	 * @return Array of localized tooltip keys.
	 */
	UFUNCTION()
	TArray<FString> GetTooltipList() const;
	
	// If true, automatically select the first valid branch instead of waiting for player input.
	UPROPERTY(EditAnywhere, Category="YADSP")
	bool bAutoChoice = false;
	
	// The list of possible dialogue options branching from this node.
	UPROPERTY(EditAnywhere, Category="YADSP")
	TArray<FBranchCondition> BranchOptions;
};
