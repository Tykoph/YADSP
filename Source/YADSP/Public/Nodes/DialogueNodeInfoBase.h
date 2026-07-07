// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueNodeInfoBase.generated.h"

/**
 * Delegate broadcast when node properties are modified in the editor.
 */
DECLARE_MULTICAST_DELEGATE(FOnPropertiesChanged);

/**
 * Base class containing data payload for a specific node in the dialogue graph.
 */
UCLASS()
class YADSP_API UDialogueNodeInfoBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UDialogueSystem> DialogueSystem = nullptr;
	
	FOnPropertiesChanged OnPropertiesChanged;
};
