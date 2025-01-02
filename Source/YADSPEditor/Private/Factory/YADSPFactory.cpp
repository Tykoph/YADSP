#include "YADSPFactory.h"
#include "DialogueGraph.h"

UYADSPFactory::UYADSPFactory(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	SupportedClass = UDialogueGraph::StaticClass();
}

// Create new instance of UDialogueGraph from the context menu in the content drawer
UObject* UYADSPFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UDialogueGraph* ProxyObj = NewObject<UDialogueGraph>(InParent, SupportedClass, InName, Flags);
	return ProxyObj;
}

bool UYADSPFactory::CanCreateNew() const
{
	return true;
}


