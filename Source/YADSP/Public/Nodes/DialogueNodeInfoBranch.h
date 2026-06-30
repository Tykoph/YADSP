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
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UFUNCTION()
	TArray<FString> GetTextList() const;
	
	UPROPERTY(EditAnywhere)
	bool bAutoChoice = false;
	
	UPROPERTY(EditAnywhere)
	TArray<FBranchCondition> BranchOptions;
};
