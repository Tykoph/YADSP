#include "DialogueGraphNode.h"

#include "DialogueSystemNodeInfo.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"

FText UDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInfoPtr->Title.IsEmpty())
	{
		FString DialogueTextStr = NodeInfoPtr->DialogueText.ToString();
		if (DialogueTextStr.Len() > 15)
		{
			DialogueTextStr = DialogueTextStr.Left(15) + TEXT("...");
		}
		return FText::FromString(DialogueTextStr);
	}

	return NodeInfoPtr->Title;
}

void UDialogueGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("Dialogue Node Actions")));

	UDialogueGraphNode* Node = const_cast<UDialogueGraphNode*>(this);
	Section.AddMenuEntry(
		"AddPinEntry",
		FText::FromString(TEXT("Add Response")),
		FText::FromString(TEXT("Create a new response on this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] (){
				Node->GetNodeInfo()->DialogueResponses.Add(FText::FromString(TEXT("New Response")));
				Node->SyncWithNodeResponse();

				Node->GetGraph()->NotifyGraphChanged();
				Node->GetGraph()->Modify();
			}
		))
	);

	Section.AddMenuEntry(
		"DeleteLastPinEntry",
		FText::FromString(TEXT("Delete Last response")),
		FText::FromString(TEXT("Delete last response on this node")),
		FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] (){
				UEdGraphPin* Pin = Node->GetPinAt(Node->Pins.Num() - 1);
				if (Pin->Direction != EGPD_Input)
				{
					UDialogueSystemNodeInfo* NodeInfo = Node->GetNodeInfo();
					NodeInfo->DialogueResponses.RemoveAt(NodeInfo->DialogueResponses.Num() - 1);
					Node->SyncWithNodeResponse();

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

void UDialogueGraphNode::SyncWithNodeResponse()
{
	UDialogueSystemNodeInfo* NodeInfo = GetNodeInfo();
	int NumGraphNodePins = Pins.Num() - 1;
	int NumInfoPins = NodeInfo->DialogueResponses.Num();

	while (NumGraphNodePins > NumInfoPins)
	{
		RemovePinAt(NumGraphNodePins-1, EGPD_Output);
		NumGraphNodePins--;
	}
	while (NumInfoPins > NumGraphNodePins)
	{
		CreateDialoguePin(EGPD_Output, FName(NodeInfo->DialogueResponses[NumGraphNodePins].ToString()));
		NumGraphNodePins++;
	}

	int Index = 1;
	for (const FText& Response : NodeInfo->DialogueResponses)
	{
		GetPinAt(Index)->PinName = FName(Response.ToString());
		Index++;
	}
}
