#include "DialogueGraphUserSettings.h"

#if WITH_EDITOR
void UDialogueGraphUserSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.Property) 
	{
		const FStructProperty* StructProp = CastField<FStructProperty>(PropertyChangedEvent.Property);
		if (StructProp && StructProp->Struct == FDialogueShortcut::StaticStruct()) {
			OnShortcutsChanged.Broadcast();
		}
	}
}
#endif