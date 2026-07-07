// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNodeInfoBase.h"
#include "DialogueNodeInfoGoTo.generated.h"

/**
 * Node info class that jumps dialogue execution directly to a specified label.
 */
UCLASS(BlueprintType)
class YADSP_API UDialogueNodeInfoGoTo : public UDialogueNodeInfoBase
{
	GENERATED_BODY()

public:
	// The target label node name to jump to.
	UPROPERTY(EditAnywhere, meta=(GetOptions="GetAllLabelNodes"), Category="YADSP")
	FName LabelName;
	
	/**
	 * Retrieves all available label names in the current graph.
	 * @return Array of label names as strings.
	 */
	UFUNCTION()
	TArray<FString> GetAllLabelNodes() const;
};
