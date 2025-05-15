// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialogueSystemFactory.h"
#include "DialogueSystem.h"

/*
 * Dialogue Actor Factory
 */
UDialogueSystemFactory::UDialogueSystemFactory(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SupportedClass = UDialogueSystem::StaticClass();
}

UObject* UDialogueSystemFactory::FactoryCreateNew(UClass* UClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UDialogueSystem* ProxyObj = NewObject<UDialogueSystem>(InParent, InName, Flags);
	return ProxyObj;
}

bool UDialogueSystemFactory::CanCreateNew() const
{
	return true;
}
