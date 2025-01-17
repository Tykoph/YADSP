#include "DialogueSystemFactory.h"
#include "DialogueSystem.h"

/*
 * Dialogue Actor Factory
 */
UDialogueSystemFactory::UDialogueSystemFactory(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SupportedClass = UDialogueSystem::StaticClass();
}

// Create new instance of UDialogueGraph from the context menu in the content drawer
UObject* UDialogueSystemFactory::FactoryCreateNew(UClass* UClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                  UObject* Context, FFeedbackContext* Warn)
{
	UDialogueSystem* ProxyObj = NewObject<UDialogueSystem>(InParent, InName, Flags);
	return ProxyObj;
}

bool UDialogueSystemFactory::CanCreateNew() const
{
	return true;
}
