// Copyright 2026 Tom Duby. All Rights Reserved.

#include "DialoguePlayer.h"

#include "DialogueSubsystem.h"
#include "DialogueSystem.h"

#include "Nodes/DialogueNodeInfoGameAction.h"
#include "Nodes/DialogueNodeInfoEnd.h"
#include "Nodes/DialogueNodeInfoText.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(DialoguePlayerSub, Log, All);

void UDialoguePlayer::PlayDialogue(UDialogueSystem* DialogueAsset, APlayerController* PlayerController, const FDialogueEndCallback& OnDialogueEnded)
{
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>(); 
	
	if (DialogueAsset == nullptr) {
		UE_LOG(DialoguePlayerSub, Error, TEXT("No dialogue asset provided"));
		return;
	}
	if (PlayerController == nullptr) {
		UE_LOG(DialoguePlayerSub, Error, TEXT("No player controller provided"));
		return;
	}
	
	OnDialogueEndedCallback = OnDialogueEnded;
	
	UDialogueSystemRuntimeGraph* Graph = DialogueAsset->Graph;

	DialogueAssetPtr = DialogueAsset;
	PlayerControllerPtr = PlayerController;

	// Find the start node
	for (UDialogueRuntimeGraphNode* Node : Graph->Nodes) {
		if (Node->NodeType == EDialogueNodeType::StartNode) {
			CurrentNodePtr = Node;
			break;
		}
	}

	if (CurrentNodePtr == nullptr) {
		UE_LOG(DialoguePlayerSub, Error, TEXT("No start node found in the dialogue graph"));
		return;
	}

	DialogueSubsystem->OnDialogueStarted.Broadcast(DialogueAsset, PlayerController);
    DialogueSubsystem->OnOptionSelected.AddDynamic(this, &UDialoguePlayer::ChooseOptionAtIndex);
	
	// Play the first node
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::ChooseOptionAtIndex(int Index)
{
	// Check if the index is valid
	if (Index >= CurrentNodePtr->OutputPins.Num() || Index < 0) {
		UE_LOG(DialoguePlayerSub, Error, TEXT("Invalid option index %d"), Index);
		return;
	}
	
	// Navigate to the selected node
	UDialogueRuntimeGraphPin* SelectedPin = CurrentNodePtr->OutputPins[Index];
	if (SelectedPin->ConnectedPin != nullptr) {
		CurrentNodePtr = SelectedPin->ConnectedPin->ParentNode;
	}
	else {
		// No Connection, this is an end node
		CurrentNodePtr = nullptr;
	}

	// If the current node is a text node, display the dialogue UI
	if (CurrentNodePtr != nullptr && CurrentNodePtr->NodeType == EDialogueNodeType::TextNode) {
		UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(CurrentNodePtr->NodeInfo);
		
		// Set the speaker name
		FString CombinedSpeakerNames;
		for (const FName& ID : NodeInfo->SpeakerIDs) {
			if (!CombinedSpeakerNames.IsEmpty()) {
				CombinedSpeakerNames += TEXT(", ");
			}
			CombinedSpeakerNames += NodeInfo->GetSpeakerName(ID);
		}

		DialogueSubsystem->OnDialogueLineRequested.Broadcast(NodeInfo->GetDialogueText(NodeInfo->DialogueID), CombinedSpeakerNames, NodeInfo->DialogueResponses);
		
		AutoSkipDialogueSelector(NodeInfo);
	}

	// If the current node is an action node, execute the action
	else if (CurrentNodePtr != nullptr && CurrentNodePtr->NodeType == EDialogueNodeType::GameActionNode) {
		FString ActionData = TEXT("");
		UDialogueNodeInfoGameAction* ActionNodeInfo = Cast<UDialogueNodeInfoGameAction>(CurrentNodePtr->NodeInfo);

		if (OnDialogueEndedCallback.IsBound()) {
			OnDialogueEndedCallback.Execute();
		}
		ChooseOptionAtIndex(0);
	}

	// If the current node is an end node, end the dialogue
	else if (CurrentNodePtr == nullptr || CurrentNodePtr->NodeType == EDialogueNodeType::EndNode) {
		if (CurrentNodePtr != nullptr) {
			UDialogueNodeInfoEnd* EndNodeInfo = Cast<UDialogueNodeInfoEnd>(CurrentNodePtr->NodeInfo);
		}

		DialogueSubsystem->OnDialogueEnded.Broadcast();
	}
}

float UDialoguePlayer::CalculateSkipTimer(const FString& Text)
{
	// Convert the FText to an FString to get its length
	const FString Buffer = Text;

	// Calculate the length of the text in seconds at a reading rate of 15 words per second
	float Length = Buffer.Len();
	Length = Length / 15;

	// Add some extra padding time before the text is automatically skipped
	Length = Length * 1.2;

	return Length;
}

void UDialoguePlayer::AutoSkipDialogue(float Time)
{
	// Set up a timer with the given time to automatically skip the dialogue
	GetWorld()->GetTimerManager().SetTimer(AutoSkipTimerHandle, [this]()
	{
		// When the timer completes, choose the first option in the current node
		ChooseOptionAtIndex(0);
	}, Time, false);
}

void UDialoguePlayer::AutoSkipDialogueSelector(const UDialogueNodeInfoText* NodeInfo)
{
	if (NodeInfo->DialogueResponses.Num() == 1) {
		switch (NodeInfo->SkipDialogue) {
			case ESkipDialogue::NoSkip:
				break;
			case ESkipDialogue::AutoSkipBasedOnText:
				CurrentSkipTime = CalculateSkipTimer(NodeInfo->GetDialogueText(NodeInfo->DialogueID));
				AutoSkipDialogue(CurrentSkipTime);
				break;
			
			case ESkipDialogue::AutoSkipAfterSound:
				if (NodeInfo->DialogueSound == nullptr) break;
				CurrentSkipTime = NodeInfo->DialogueSound->GetDuration();
				
				// TODO: see how to properly play sound
				UGameplayStatics::PlaySound2D(GetWorld(), NodeInfo->DialogueSound);
				AutoSkipDialogue(CurrentSkipTime);
				break;
			
			case ESkipDialogue::AutoSkipAfterTime:
				CurrentSkipTime = NodeInfo->SkipAfterSeconds;
				AutoSkipDialogue(CurrentSkipTime);
				break;
		}
	}
}
