// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogueSystemFactory.generated.h"

/**
 * Factory class for creating new instances of UDialogueSystem assets.
 * 
 * This factory is responsible for handling the creation of new dialogue system assets
 * within the Unreal Editor. It provides the necessary implementation for creating
 * new UDialogueSystem objects through the content browser or other asset creation workflows.
 */
UCLASS()
class YADSPEDITOR_API UDialogueSystemFactory : public UFactory
{
	GENERATED_BODY()

public:
	explicit UDialogueSystemFactory(const FObjectInitializer& ObjectInitializer);
	
	/** Creates the actual UDialogueSystem object instance. */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	
	/** Returns whether this factory is currently allowed to create new objects. */
	virtual bool CanCreateNew() const override;
};
