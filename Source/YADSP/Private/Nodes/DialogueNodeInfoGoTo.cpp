// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoGoTo.h"
#include "Nodes/DialogueNodeInfoLabel.h"

TArray<FString> UDialogueNodeInfoGoTo::GetAllLabelNodes() const
{
	TArray<FString> Result;
	for (const UDialogueRuntimeGraphNode* Node : DialogueSystem->Graph->Nodes) {
		if (Node->NodeType == EDialogueNodeType::LabelNode){
			Result.Add(Cast<UDialogueNodeInfoLabel>(Node->NodeInfo)->LabelName);
		}
	}
	
	return Result;
}
