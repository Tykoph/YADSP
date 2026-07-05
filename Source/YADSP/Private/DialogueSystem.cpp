// Copyright Tom Duby. All Rights Reserved.

#include "DialogueSystem.h"
#include "UObject/ObjectSaveContext.h"

#if WITH_EDITOR
void UDialogueSystem::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	if (OnPreSaveListenerPtr) {
		OnPreSaveListenerPtr();
	}
}
#endif
