#include "DialogueGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"

void UDialogueGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Dialogue Node Actions")));

	UDialogueGraphNode* Node = const_cast<UDialogueGraphNode*>(this);
	Section.AddMenuEntry(
		"AddPinEntry",
		FText::FromString(TEXT("Add Pin")),
		FText::FromString(TEXT("Create a new pin on this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] (){
				Node->CreateDialoguePin(
					EGPD_Output,
					TEXT("another output")
				);

				Node->GetGraph()->NotifyGraphChanged();
				Node->GetGraph()->Modify();
			}
		))
	);

	Section.AddMenuEntry(
		"DeleteLastPinEntry",
		FText::FromString(TEXT("Delete Last Pin")),
		FText::FromString(TEXT("Delete last pin on this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] (){
				UEdGraphPin* Pin = Node->GetPinAt(Node->Pins.Num() - 1);
				if (Pin->Direction != EGPD_Input)
				{
					Node->RemovePin(Pin);

					Node->GetGraph()->NotifyGraphChanged();
					Node->GetGraph()->Modify();
				}
			}
		))
	);

	Section.AddMenuEntry(
		"DeleteEntry",
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Delete this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeleteNodeIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] (){
				Node->DestroyNode();
				Node->GetGraph()->NotifyGraphChanged();
				Node->GetGraph()->Modify();
			}
		))
	);
}

UEdGraphPin* UDialogueGraphNode::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName PinCategory = (Dir == EGPD_Input) ? TEXT("input") : TEXT("output");
	FName PinSubCategory = TEXT("DialoguePin");

	UEdGraphPin* NewPin = CreatePin(Dir ,PinCategory, PinSubCategory, Name);
	NewPin->PinType.PinCategory = PinCategory;

	return NewPin;
}
