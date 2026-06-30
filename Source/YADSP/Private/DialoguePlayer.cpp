// Copyright Tom Duby. All Rights Reserved.

#include "DialoguePlayer.h"

#include "DialogueNodeType.h"
#include "DialogueSubsystem.h"
#include "DialogueSystem.h"
#include "DialogueSystemLibrary.h"
#include "YADSP.h"

#include "Nodes/DialogueNodeInfoGameAction.h"
#include "Nodes/DialogueNodeInfoText.h"
#include "Nodes/DialogueNodeInfoBranch.h"
#include "Nodes/DialogueNodeInfoGoTo.h"
#include "Nodes/DialogueNodeInfoLabel.h"

#include "RuntimeGraph/DialogueRuntimeGraphNode.h"
#include "RuntimeGraph/DialogueRuntimeGraphPin.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UDialoguePlayer::PlayDialogue(UDialogueSystem* InDialogueAsset, APlayerController* InPlayerController, const FDialogueEndCallback OnDialogueEnded)
{
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>(); 
	GameActionSubsystem = GetWorld()->GetSubsystem<UGameActionSubsystem>(); 
	
	if (DialogueSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::PlayDialogue -> DialogueSubsystem not found"));
		return;
	}
	if (GameActionSubsystem == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::PlayDialogue -> GameActionSubsystem not found"));
		return;
	}
	if (InDialogueAsset == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::PlayDialogue -> No dialogue asset provided"));
		return;
	}
	if (InPlayerController == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::PlayDialogue -> No player controller provided"));
		return;
	}
	
	OnDialogueEndedCallback = OnDialogueEnded;
	
	UDialogueSystemRuntimeGraph* Graph = InDialogueAsset->Graph;
	if (Graph == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::PlayDialogue -> Graph is null"));
		return;
	}
	
	DialogueSystem = InDialogueAsset;
	this->PlayerController = InPlayerController;

	// Find the start node
	for (UDialogueRuntimeGraphNode* Node : Graph->Nodes) {
		if (Node->NodeType == EDialogueNodeType::StartNode) {
			CurrentNode = Node;
			break;
		}
	}

	if (CurrentNode == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::PlayDialogue -> No start node found in the dialogue graph"));
		return;
	}

	DialogueSubsystem->OnDialogueStarted.Broadcast(InDialogueAsset, InPlayerController);
    DialogueSubsystem->OnOptionSelected.AddUniqueDynamic(this, &UDialoguePlayer::ChooseOptionAtIndex);
	
	// Play the first node
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::ChooseOptionAtIndex(const int InIndex)
{
	if (CurrentNode == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ChooseOptionAtIndex -> Current Node is nullptr"));
		return;
	}
	
	if (InIndex >= CurrentNode->OutputPins.Num() || InIndex < 0) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ChooseOptionAtIndex -> Invalid option index %d"), InIndex);
		return;
	}
	
	// Navigate to the selected node
	const UDialogueRuntimeGraphPin* SelectedPin = CurrentNode->OutputPins[InIndex];
	if (SelectedPin->ConnectedPin != nullptr) {
		CurrentNode = SelectedPin->ConnectedPin->ParentNode;
	}
	else {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ChooseOptionAtIndex -> Current Node is null, should be type EndNode type, go into the DialogueAsset to fix it. Ending dialogue"));
		FinishDialogue();
		return;
	}
	
	switch (CurrentNode->NodeType) {
		case EDialogueNodeType::StartNode:
			ChooseOptionAtIndex(0);
			break;
		case EDialogueNodeType::EndNode:
			FinishDialogue();
			break;
		case EDialogueNodeType::TextNode:
			ProcessTextNode();
			break;
		case EDialogueNodeType::BranchNode:
			ProcessBranchNode();
			break;
		case EDialogueNodeType::GameActionNode:
			ProcessGameActionNode();
			break;
		case EDialogueNodeType::GoToNode:
			ProcessGoToNode();
			break;
		case EDialogueNodeType::LabelNode:
			ChooseOptionAtIndex(0);
			break;
		default:
			UE_LOG(LogYADSP, Warning, TEXT("UDialoguePlayer::ChooseOptionAtIndex -> Current NodeType is null, should be a valid type, Ending dialogue"));
			FinishDialogue();
	}
}

void UDialoguePlayer::ProcessTextNode()
{
	const UDialogueNodeInfoText* TextNodeInfo = Cast<UDialogueNodeInfoText>(CurrentNode->NodeInfo);
	if (TextNodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessTextNode -> TextNodeInfo is nullptr"));
		return;
	}
		
	// Set the speaker name
	FString CombinedSpeakerNames;
	for (const FName& SpeakerKey : TextNodeInfo->SpeakerKeys) {
		if (!CombinedSpeakerNames.IsEmpty()) {
			CombinedSpeakerNames += TEXT(", ");
		}
		CombinedSpeakerNames += UDialogueSystemLibrary::GetTranslatedText(DialogueSystem, DialogueSystem->SpeakerDataTable, SpeakerKey);
	}

	DialogueSubsystem->OnDialogueLineRequested.Broadcast(
		FText::FromString(UDialogueSystemLibrary::GetTranslatedText(DialogueSystem, DialogueSystem->DialogueDataTable, TextNodeInfo->DialogueKey)),
		FText::FromString(CombinedSpeakerNames)
		);
		
	ProcessDialogueAutoSkip(TextNodeInfo);
}

void UDialoguePlayer::ProcessBranchNode()
{
	const UDialogueNodeInfoBranch* BranchNodeInfo = Cast<UDialogueNodeInfoBranch>(CurrentNode->NodeInfo);
	if (BranchNodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessBranchNode -> BranchNodeInfo is nullptr"));
		return;
	}
			
	TArray<FBranchOption> BranchToDisplay;
	for (int i = 0; i < BranchNodeInfo->BranchOptions.Num(); ++i) {
		bool bIsValid;
				
		if (BranchNodeInfo->BranchOptions[i].Expression) {
			BranchNodeInfo->BranchOptions[i].Expression->ExecuteAction();
			bIsValid = BranchNodeInfo->BranchOptions[i].Expression->GetResult();
		}
		else 
			bIsValid = true;
				
		if (!bIsValid && BranchNodeInfo->BranchOptions[i].bHideIfExpressionFail)
			continue;
				
		if (BranchNodeInfo->bAutoChoice && bIsValid) {
			ChooseOptionAtIndex(i);
			break;
		}
				
		FBranchOption CurrentBranch;
		CurrentBranch.DialogueText = FText::FromString(UDialogueSystemLibrary::GetTranslatedText(
				DialogueSystem, DialogueSystem->DialogueDataTable, BranchNodeInfo->BranchOptions[i].DialogueResponseKey));
		CurrentBranch.Tooltip = FText::FromString(UDialogueSystemLibrary::GetTranslatedText(
				DialogueSystem, DialogueSystem->DialogueDataTable, BranchNodeInfo->BranchOptions[i].ConditionTooltipKey));
		CurrentBranch.bExpressionIsValid = bIsValid;
		BranchToDisplay.Add(CurrentBranch);
	}
			
	DialogueSubsystem->OnBranchOptionsRequested.Broadcast(BranchToDisplay);
}

void UDialoguePlayer::ProcessGameActionNode()
{
	const UDialogueNodeInfoGameAction* GameActionNodeInfo = Cast<UDialogueNodeInfoGameAction>(CurrentNode->NodeInfo);
	if (GameActionNodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessGameActionNode -> GameActionNodeInfo is nullptr"));
		return;
	}
		
	if (GameActionNodeInfo->GameActions.Num() > 0) {
		InstancedActions.Empty();
		InstancedActions.Reserve(GameActionNodeInfo->GameActions.Num());
			
		for (const UGameActionBase* ActionTemplate : GameActionNodeInfo->GameActions) {
			if (!ActionTemplate) {
				UE_LOG(LogYADSP, Warning, TEXT("UDialoguePlayer::ProcessGameActionNode -> ActionTemplate is null"))
				continue;
			}
			UGameActionBase* InstancedAction = DuplicateObject<UGameActionBase>(ActionTemplate, this);
			InstancedActions.Add(InstancedAction);
		}
			
		if (InstancedActions.Num() == 1) {
			FOnGameActionCompleted OnCompleted;
			OnCompleted.BindDynamic(this, &UDialoguePlayer::OnGameActionFinished);
			if (InstancedActions.Num() == 1) 
				GameActionSubsystem->ExecuteGameAction(InstancedActions[0], OnCompleted);
			else
				UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessGameActionNode ->InstancedActions[0] is nullptr"));
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
			default:
				UE_LOG(LogYADSP, Warning, TEXT("UDialoguePlayer::ProcessGameActionNode -> Unknown Game Action ExecutionMode"));
			}
		}
	}
	else {
		ChooseOptionAtIndex(0);
	}
}

void UDialoguePlayer::ProcessGoToNode()
{
	const UDialogueNodeInfoGoTo* GoToNodeInfo = Cast<UDialogueNodeInfoGoTo>(CurrentNode->NodeInfo);
	if (GoToNodeInfo == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessGoToNode -> GoToNodeInfo is nullptr"));
		return;
	}
	
	if (DialogueSystem == nullptr || DialogueSystem->Graph == nullptr) {
		UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::Proces -> %s%s"), 
			(DialogueSystem ? TEXT("") : TEXT("DialogueSystem is Null. ")),
			(DialogueSystem->Graph ? TEXT("") : TEXT("DialogueSystem->Graph is Null. "))
			);
		return;
	}
	
	for (UDialogueRuntimeGraphNode* Node : DialogueSystem->Graph->Nodes) {
		if (const auto* LabelInfo = Cast<UDialogueNodeInfoLabel>(Node->NodeInfo)) {
			if (LabelInfo->LabelName == GoToNodeInfo->LabelName) {
				CurrentNode = Node;
				ChooseOptionAtIndex(0);
				return;
			}
		}
	}

	UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessGoToNode -> No Label found with name %s, Ending Dialogue"), *GoToNodeInfo->LabelName.ToString());
	FinishDialogue();
}

void UDialoguePlayer::FinishDialogue()
{
	DialogueSubsystem->OnOptionSelected.RemoveDynamic(this, &UDialoguePlayer::ChooseOptionAtIndex);
	DialogueSubsystem->OnDialogueEnded.Broadcast();
	OnDialogueEndedCallback.ExecuteIfBound();
}

// TODO: Find a better way to calculate the timer based on text length (especially for non-alphabetic languages)
float UDialoguePlayer::CalculateSkipTimer(const FString& InText)
{
	// Calculate the length of the text in seconds at a reading rate of 15 words per second
	float Length = InText.Len();
	Length = Length / 15;

	// Add some extra padding time before the text is automatically skipped
	Length = Length * 1.2;

	return Length;
}

void UDialoguePlayer::AutoSkipDialogue(const float InTime)
{
	GetWorld()->GetTimerManager().SetTimer(AutoSkipTimerHandle, this, &UDialoguePlayer::ExecuteAutoSkip, InTime, false);
}

void UDialoguePlayer::ExecuteAutoSkip()
{
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::OnGameActionFinished()
{
	for (UGameActionBase* Action : InstancedActions) {
		if (Action)
			Action->MarkAsGarbage();
	}

	InstancedActions.Empty();
	
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::ProcessDialogueAutoSkip(const UDialogueNodeInfoText* InNodeInfo)
{
	switch (InNodeInfo->SkipDialogue) {
		case ESkipDialogue::NoSkip:
			break;
		
		case ESkipDialogue::AutoSkipBasedOnText:
			CurrentSkipTime = CalculateSkipTimer(UDialogueSystemLibrary::GetTranslatedText(DialogueSystem, DialogueSystem->DialogueDataTable, InNodeInfo->DialogueKey));
			AutoSkipDialogue(CurrentSkipTime);
			break;
		
		case ESkipDialogue::AutoSkipAfterSound:
			if (InNodeInfo->DialogueSound == nullptr) {
				UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessDialogueAutoSkip -> No DialogueSound provided"));
				break;
			}
			CurrentSkipTime = InNodeInfo->DialogueSound->GetDuration();
			
			// TODO: Make a custom sound player to replace this simple implementation (optional for)
			UGameplayStatics::PlaySound2D(GetWorld(), InNodeInfo->DialogueSound);
			AutoSkipDialogue(CurrentSkipTime);
			break;
		
		case ESkipDialogue::AutoSkipAfterTime:
			CurrentSkipTime = InNodeInfo->SkipAfterSeconds;
			AutoSkipDialogue(CurrentSkipTime);
			break;

		default:
			UE_LOG(LogYADSP, Error, TEXT("UDialoguePlayer::ProcessDialogueAutoSkip -> Unknown Skip Dialogue"));
			break;
	}
}

TArray<FText> UDialoguePlayer::ConvertStringArrayToTextArray(const TArray<FString>& InStringArray)
{
	TArray<FText> Result;

	Result.Reserve(InStringArray.Num());
	
	for (const FString& String : InStringArray) {
		Result.Add(FText::FromString(String));
	}
	
	return Result;
}
