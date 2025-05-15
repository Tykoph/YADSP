// Copyright 2025 Tom Duby. All Rights Reserved.

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
	/**
	 * Constructor for the DialogueSystemFactory.
	 * 
	 * @param ObjectInitializer The standard object initializer for Unreal objects.
	 */
	UDialogueSystemFactory(const FObjectInitializer& ObjectInitializer);

	/**
	 * Creates a new instance of UDialogueSystem.
	 *
	 * @param UClass The class type of the object to create. This should be UDialogueSystem or a subclass.
	 * @param InParent The parent object for the newly created object.
	 * @param InName The name for the new object.
	 * @param Flags The object flags to apply to the new object.
	 * @param Context Optional context for creation, may be null.
	 * @param Warn Feedback context for warning messages.
	 * @return A pointer to the newly created UDialogueSystem object.
	 */
	virtual UObject* FactoryCreateNew(UClass* UClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	/**
	 * Checks if this factory can create new instances.
	 * 
	 * @return True if the factory can create new instances, false otherwise.
	 */
	virtual bool CanCreateNew() const override;
};
