#include "DialogueActorFactory.h"
#include "DialogueActor.h"

/*
 * Dialogue Actor Factory
 */
UDialogueActorFactory::UDialogueActorFactory(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SupportedClass = ADialogueActor::StaticClass();
}

// Create new instance of UDialogueGraph from the context menu in the content drawer
UObject* UDialogueActorFactory::FactoryCreateNew(UClass* UClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	ADialogueActor* ProxyObj = NewObject<ADialogueActor>(InParent, InName, Flags);
	return ProxyObj;
}

bool UDialogueActorFactory::CanCreateNew() const
{
	return true;
}
