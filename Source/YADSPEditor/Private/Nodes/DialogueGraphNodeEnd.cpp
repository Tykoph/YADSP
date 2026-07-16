// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueGraphNodeEnd.h"

#include "YADSP.h"

FText UDialogueGraphNodeEnd::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End"));
}

bool UDialogueGraphNodeEnd::CanUserDeleteNode() const
{
	const UEdGraph* CurrentGraph = GetGraph();
	if (CurrentGraph == nullptr) {
		UE_LOG(LogYADSP, Log, TEXT("UDialogueGraphNodeEnd::CanUserDeleteNode -> Current Graph is nullptr"));
		return false;
	}

	int i = 0;
	for (UEdGraphNode* Node : CurrentGraph->Nodes) {
		if (Cast<UDialogueGraphNodeEnd>(Node)) {
			i++;
			// Ensure that at least one End node remains in the graph; allow deletion only if more than 1 exists
			if (i > 1)
				return true;
		}
	}
	
	return false;
}

void UDialogueGraphNodeEnd::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("DialogueSection"), FText::FromString(TEXT("End Node Actions")));

	const TWeakObjectPtr WeakNode = const_cast<UDialogueGraphNodeEnd*>(this);
	if (auto* Node = WeakNode.Get()) {
		Section.AddMenuEntry(
			"DeleteEntry",
			FText::FromString(TEXT("Delete Node")),
			FText::FromString(TEXT("Delete this node")),
			FSlateIcon(TEXT("YADSPStyle"), TEXT("DialogueGraphEditor.NodeDeleteNodeIcon")),
			FUIAction(
				FExecuteAction::CreateLambda([Node]()
				{
					const FScopedTransaction Transaction(FText::FromString(TEXT("Delete Node")));
					Node->GetGraph()->Modify();
					Node->Modify();
					Node->GetGraph()->RemoveNode(Node);
				}),
				FCanExecuteAction::CreateLambda([Node]() { return Node->CanUserDeleteNode(); })
			)
		);
	}
}

UEdGraphPin* UDialogueGraphNodeEnd::CreateDefaultInputPin()
{
	return CreateDialoguePin(
		EGPD_Input, 
		FName(TEXT("Finish")), 
		FName(TEXT("Input"))
		);
}
