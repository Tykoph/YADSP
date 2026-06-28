// Copyright Tom Duby. All Rights Reserved.

#include "DialogueSystem.h"
#include "UObject/ObjectSaveContext.h"

void UDialogueSystem::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);
	if (OnPreSaveListenerPtr) {
		OnPreSaveListenerPtr();
	}
}
