// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "DialogueNodeInfoBase.h"
#include "BranchCondition.h"
#include "DialogueNodeInfoBranch.generated.h"

UCLASS()
class YADSP_API UDialogueNodeInfoBranch : public UDialogueNodeInfoBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	bool bAutoChoice = false;
	
	UPROPERTY(EditAnywhere)
	TArray<FBranchCondition> BranchOptions;
	
	UFUNCTION()
	TArray<FString> GetDialogue() const;
	UFUNCTION()
	TArray<FString> GetTooltip() const;
};
