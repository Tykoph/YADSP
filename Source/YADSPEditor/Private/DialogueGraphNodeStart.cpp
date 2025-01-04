#include "DialogueGraphNodeStart.h"

UEdGraphPin* UDialogueGraphNodeStart::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName PinCategory = TEXT( "Output" );
	FName PinSubCategory = TEXT("StartPin");

	UEdGraphPin* NewPin = CreatePin(Dir ,PinCategory, PinSubCategory, Name);
	NewPin->PinType.PinCategory = PinCategory;

	return NewPin;
}
