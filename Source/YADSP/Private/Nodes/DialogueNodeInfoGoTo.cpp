// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoGoTo.h"

#include "DialogueNodeType.h"
#include "YADSP.h"
#include "Nodes/DialogueNodeInfoLabel.h"
#include "RuntimeGraph/DialogueRuntimeGraphNode.h"

TArray<FString> UDialogueNodeInfoGoTo::GetAllLabelNodes() const
{
	TArray<FString> Result;
	if (DialogueSystem == nullptr || DialogueSystem->Graph == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialogueNodeInfoGoTo::GetAllLabelNodes -> %s%s"), 
			(DialogueSystem ? TEXT("") : TEXT("DialogueSystem is Null. ")),
			(DialogueSystem->Graph ? TEXT("") : TEXT("DialogueSystem->Graph is Null. "))
			);
		return Result;
	}
	
	for (const UDialogueRuntimeGraphNode* Node : DialogueSystem->Graph->Nodes) {
		if (Node->NodeType == EDialogueNodeType::LabelNode){
			Result.Add(Cast<UDialogueNodeInfoLabel>(Node->NodeInfo)->LabelName.ToString());
		}
	}
	
	return Result;
}
