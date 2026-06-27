// Copyright Tom Duby. All Rights Reserved.

#include "DialoguePlayer.h"

#include "DialogueNodeType.h"
#include "DialogueSubsystem.h"
#include "DialogueSystem.h"
#include "DialogueSystemLibrary.h"

#include "Nodes/DialogueNodeInfoGameAction.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "Nodes/DialogueNodeInfoBranch.h"
#include "Nodes/DialogueNodeInfoGoTo.h"
#include "Nodes/DialogueNodeInfoLabel.h"

#include "RuntimeGraph/DialogueRuntimeGraphNode.h"
#include "RuntimeGraph/DialogueRuntimeGraphPin.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogDialoguePlayer, Log, All);

void UDialoguePlayer::PlayDialogue(UDialogueSystem* DialogueAsset, APlayerController* PlayerController, const FDialogueEndCallback& OnDialogueEnded)
{
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>(); 
	GameActionSubsystem = GetWorld()->GetSubsystem<UGameActionSubsystem>(); 
	
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("DialogueSubsystem not found"));
		return;
	}
	if (GameActionSubsystem == nullptr) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("GameActionSubsystem not found"));
		return;
	}
	if (DialogueAsset == nullptr) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("No dialogue asset provided"));
		return;
	}
	if (PlayerController == nullptr) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("No player controller provided"));
		return;
	}
	
	OnDialogueEndedCallback = OnDialogueEnded;
	
	UDialogueSystemRuntimeGraph* Graph = DialogueAsset->Graph;

	DialogueSystem = DialogueAsset;
	this->PlayerController = PlayerController;

	// Find the start node
	for (UDialogueRuntimeGraphNode* Node : Graph->Nodes) {
		if (Node->NodeType == EDialogueNodeType::StartNode) {
			CurrentNode = Node;
			break;
		}
	}

	if (CurrentNode == nullptr) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("No start node found in the dialogue graph"));
		return;
	}

	DialogueSubsystem->OnDialogueStarted.Broadcast(DialogueAsset, PlayerController);
    DialogueSubsystem->OnOptionSelected.AddDynamic(this, &UDialoguePlayer::ChooseOptionAtIndex);
	
	// Play the first node
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::ChooseOptionAtIndex(int Index)
{
	if (!CurrentNode.IsValid()) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("Current Node Invlide"));
		return;
	}
	
	if (Index >= CurrentNode->OutputPins.Num() || Index < 0) {
		UE_LOG(LogDialoguePlayer, Error, TEXT("Invalid option index %d"), Index);
		return;
	}
	
	// Navigate to the selected node
	const UDialogueRuntimeGraphPin* SelectedPin = CurrentNode->OutputPins[Index];
	if (SelectedPin->ConnectedPin != nullptr) {
		CurrentNode = SelectedPin->ConnectedPin->ParentNode;
	}
	else {
		// No Connection, this is an end node
		CurrentNode = nullptr;
	}

	// If the current node is a text node, display the dialogue UI
	if (CurrentNode != nullptr && CurrentNode->NodeType == EDialogueNodeType::TextNode) {
		UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(CurrentNode->NodeInfo);
		
		// Set the speaker name
		FString CombinedSpeakerNames;
		for (const FName& SpeakerKey : NodeInfo->SpeakerKeys) {
			if (!CombinedSpeakerNames.IsEmpty()) {
				CombinedSpeakerNames += TEXT(", ");
			}
			CombinedSpeakerNames += UDialogueSystemLibrary::GetTranslatedText(DialogueSystem, DialogueSystem->DialogueDataTable, SpeakerKey);
		}

		DialogueSubsystem->OnDialogueLineRequested.Broadcast(
			FText::FromString(UDialogueSystemLibrary::GetTranslatedText(DialogueSystem, DialogueSystem->DialogueDataTable, NodeInfo->DialogueKey)),
			FText::FromString(CombinedSpeakerNames)
			);
		
		ProcessDialogueAutoSkip(NodeInfo);
	}
	
	// If the current node is a Branch Node, Display dialogue options if applicable
	else if (CurrentNode == nullptr || CurrentNode->NodeType == EDialogueNodeType::BranchNode) {
		const UDialogueNodeInfoBranch* BranchNodeInfo = Cast<UDialogueNodeInfoBranch>(CurrentNode->NodeInfo);
		
		TArray<FBranchCondition> BranchCache = BranchNodeInfo->BranchOptions;
		
		TArray<FBranchOption> BranchToDisplay;
		for (int i = 0; i < BranchCache.Num(); ++i) {
			bool bIsValid;
			
			if (BranchCache[i].Expression)
				bIsValid = BranchCache[i].Expression->ExecuteWithReturn();
			else 
				bIsValid = true;
			
			if (!bIsValid && BranchCache[i].bHideIfExpressionFail)
				continue;
			
			if (BranchNodeInfo->bAutoChoice && bIsValid) {
				ChooseOptionAtIndex(i);
				break;
			}
			
			FBranchOption CurrentBranch;
			CurrentBranch.DialogueText = FText::FromString(UDialogueSystemLibrary::GetTranslatedText(
					DialogueSystem, DialogueSystem->DialogueDataTable, BranchCache[i].DialogueResponseKey));
			CurrentBranch.Tooltip = FText::FromString(UDialogueSystemLibrary::GetTranslatedText(
					DialogueSystem, DialogueSystem->DialogueDataTable, BranchCache[i].ConditionTooltipKey));
			CurrentBranch.bExpressionIsValid = bIsValid;
			BranchToDisplay.Add(CurrentBranch);
		}
		
		DialogueSubsystem->OnBranchOptionsRequested.Broadcast(BranchToDisplay);
	}
	
	// If the current node is an action node, execute the action
	else if (CurrentNode != nullptr && CurrentNode->NodeType == EDialogueNodeType::GameActionNode) {
		const UDialogueNodeInfoGameAction* GameActionNodeInfo = Cast<UDialogueNodeInfoGameAction>(CurrentNode->NodeInfo);

		if (GameActionNodeInfo->GameAction.Num() > 0) {
				
			TArray<UGameActionBase*> InstancedActions;
			for (const UGameActionBase* ActionTemplate : GameActionNodeInfo->GameAction) {
				if (ActionTemplate) {
					UGameActionBase* InstancedAction = DuplicateObject<UGameActionBase>(ActionTemplate, this);
					InstancedActions.Add(InstancedAction);
				}
			}
			
			if (GameActionNodeInfo->GameAction.Num() == 1) {
				FOnGameActionCompleted OnCompleted;
				OnCompleted.BindDynamic(this, &UDialoguePlayer::OnGameActionFinished);
				
				GameActionSubsystem->ExecuteGameAction(InstancedActions[0], OnCompleted);
			}
			else {
				FOnGameActionSequenceCompleted OnSequenceCompleted;
				OnSequenceCompleted.BindDynamic(this, &UDialoguePlayer::OnGameActionFinished);
			
				FOnParallelGameActionCompleted OnParallelCompleted;
				OnParallelCompleted.BindDynamic(this, &UDialoguePlayer::OnGameActionFinished);
				
				switch (GameActionNodeInfo->GameActionExecutionMode) {
					case EGameActionExecutionMode::Sequence:
						GameActionSubsystem->ExecuteGameActionSequence(InstancedActions, OnSequenceCompleted);
						break;
					case EGameActionExecutionMode::Parallel:
						GameActionSubsystem->ExecuteParallelGameAction(InstancedActions, OnParallelCompleted);
						break;
				}
			}
		}
		else {
			ChooseOptionAtIndex(0);
		}
	}

	else if (CurrentNode != nullptr && CurrentNode->NodeType == EDialogueNodeType::GoToNode) {
		const UDialogueNodeInfoGoTo* GameActionNodeInfo = Cast<UDialogueNodeInfoGoTo>(CurrentNode->NodeInfo);
		GoToNode(GameActionNodeInfo->LabelNode);
	}
	
	else if (CurrentNode != nullptr && CurrentNode->NodeType == EDialogueNodeType::LabelNode) {
		ChooseOptionAtIndex(0);
	}
	
	// If the current node is an end node, end the dialogue
	else if (CurrentNode == nullptr || CurrentNode->NodeType == EDialogueNodeType::EndNode) {
		DialogueSubsystem->OnOptionSelected.RemoveDynamic(this, &UDialoguePlayer::ChooseOptionAtIndex);
		DialogueSubsystem->OnDialogueEnded.Broadcast();
	}
}

void UDialoguePlayer::GoToNode(const FName& NodeName)
{
	// Find the start node
	for (UDialogueRuntimeGraphNode* Node : DialogueSystem->Graph->Nodes) {
		if (Node->NodeType == EDialogueNodeType::LabelNode && Cast<UDialogueNodeInfoLabel>(Node->NodeInfo)->LabelName == NodeName) {
			CurrentNode = Node;
			ChooseOptionAtIndex(0);
			return;
		}
	}

	UE_LOG(LogDialoguePlayer, Error, TEXT("No Label found with name %s, Ending Dialogue"), *NodeName.ToString());
	DialogueSubsystem->OnOptionSelected.RemoveDynamic(this, &UDialoguePlayer::ChooseOptionAtIndex);
	DialogueSubsystem->OnDialogueEnded.Broadcast();
}

// TODO: Find a better way to calculate the timer based on text lenght (especially for non-alphabetic languages)
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

void UDialoguePlayer::AutoSkipDialogue(const float Time)
{
	GetWorld()->GetTimerManager().SetTimer(AutoSkipTimerHandle, [this]()
	{
		ChooseOptionAtIndex(0);
	}, Time, false);
}

void UDialoguePlayer::OnGameActionFinished()
{
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::ProcessDialogueAutoSkip(const UDialogueNodeInfoText* NodeInfo)
{
	switch (NodeInfo->SkipDialogue) {
		case ESkipDialogue::NoSkip:
			break;
		case ESkipDialogue::AutoSkipBasedOnText:
			CurrentSkipTime = CalculateSkipTimer(UDialogueSystemLibrary::GetTranslatedText(DialogueSystem, DialogueSystem->DialogueDataTable, NodeInfo->DialogueKey));
			AutoSkipDialogue(CurrentSkipTime);
			break;
		
		case ESkipDialogue::AutoSkipAfterSound:
			if (NodeInfo->DialogueSound == nullptr) break;
			CurrentSkipTime = NodeInfo->DialogueSound->GetDuration();
			
			// TODO: Make a custom sound player to replace this simple implementation (optional for)
			UGameplayStatics::PlaySound2D(GetWorld(), NodeInfo->DialogueSound);
			AutoSkipDialogue(CurrentSkipTime);
			break;
		
		case ESkipDialogue::AutoSkipAfterTime:
			CurrentSkipTime = NodeInfo->SkipAfterSeconds;
			AutoSkipDialogue(CurrentSkipTime);
			break;
	}
}

TArray<FText> UDialoguePlayer::ConvertStringArrayToTextArray(const TArray<FString>& StringArray)
{
	TArray<FText> Result;

	for (FString& String : StringArray) {
		Result.Add(FText::FromString(String));
	}
	
	return Result;
}
