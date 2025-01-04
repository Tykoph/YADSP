#include "DialogueSystem.h"
#include "UObject/ObjectSaveContext.h"

void UDialogueSystem::PreSave(FObjectPreSaveContext SaveContext)
{
	if (OnPreSaveListenerPtr)
	{
		OnPreSaveListenerPtr();
	}
}
