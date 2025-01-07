#include "DialogueGraphNode.h"
#include "DialogueNodeInfo.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenus.h"

FText UDialogueGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UDialogueNodeInfo* NodeInfo = Cast<UDialogueNodeInfo>(NodeInfoPtr);
	if (NodeInfo->Title.IsEmpty())
	{
		FString DialogueTextStr = NodeInfo->DialogueText.ToString();
		if (DialogueTextStr.Len() > 15)
		{
			DialogueTextStr = DialogueTextStr.Left(15) + TEXT("...");
		}
		return FText::FromString(DialogueTextStr);
	}

	return NodeInfo->Title;
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
				Node->GetDialogueNodeInfo()->DialogueResponses.Add(FText::FromString(TEXT("New Response")));
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
					UDialogueNodeInfo* NodeInfo = Node->GetDialogueNodeInfo();
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
				Node->GetGraph()->RemoveNode(Node);
			}
		))
	);
}

UEdGraphPin* UDialogueGraphNode::CreateDialoguePin(EEdGraphPinDirection Dir, FName Name)
{
	FName PinCategory = (Dir == EGPD_Input) ? TEXT("input") : TEXT("output");
	FName PinSubCategory = TEXT("DialoguePin");

	UEdGraphPin* NewPin = CreatePin(Dir ,PinCategory, PinSubCategory, Name);
	NewPin->PinType.PinSubCategory = PinSubCategory;

	return NewPin;
}

UEdGraphPin* UDialogueGraphNode::CreateDefaultInputPin()
{
	return CreateDialoguePin(EGPD_Input, FName(TEXT("Input")));
}

void UDialogueGraphNode::CreateDefaultOutputPin()
{
	FString DefaultResponse = TEXT("Continue");
	CreateDialoguePin(EGPD_Output, FName(DefaultResponse));
	GetDialogueNodeInfo()->DialogueResponses.Add(FText::FromString(DefaultResponse));
}

void UDialogueGraphNode::SyncWithNodeResponse()
{
	UDialogueNodeInfo* NodeInfo = GetDialogueNodeInfo();
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
