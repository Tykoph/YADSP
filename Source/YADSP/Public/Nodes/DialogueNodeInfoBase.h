// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystem.h"
#include "DialogueNodeInfoBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPropertiesChanged);

UCLASS()
class YADSP_API UDialogueNodeInfoBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UDialogueSystem> DialogueSystem = nullptr;
	
	FOnPropertiesChanged OnPropertiesChanged;
};
