// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoGameAction.h"

#if WITH_EDITOR
void UDialogueNodeInfoGameAction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UDialogueNodeInfoGameAction, GameActions)) {
		if (GameActions.Num() < 1) {
			GameActions.SetNum(1);
		}
	}
}
#endif
