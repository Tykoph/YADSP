// Copyright Tom Duby. All Rights Reserved.

#include "DialoguePlayer.h"

#include "DialogueSubsystem.h"
#include "DialogueSystem.h"
#include "GSheetLocSystemLibrary.h"

#include "Nodes/DialogueNodeInfoGameAction.h"
#include "Nodes/DialogueNodeInfoText.h"

#include "Kismet/GameplayStatics.h"
#include "Nodes/DialogueNodeInfoBranch.h"
#include "Nodes/DialogueNodeInfoGoTo.h"
#include "Nodes/DialogueNodeInfoLabel.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(DialoguePlayerSub, Log, All);

void UDialoguePlayer::PlayDialogue(UDialogueSystem* DialogueAsset, APlayerController* PlayerController, const FDialogueEndCallback& OnDialogueEnded)
{
	DialogueSubsystem = GetWorld()->GetSubsystem<UDialogueSubsystem>(); 
	GameActionSubsystem = GetWorld()->GetSubsystem<UGameActionSubsystem>(); 

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
	if (Index >= CurrentNode->OutputPins.Num() || Index < 0) {
		UE_LOG(DialoguePlayerSub, Error, TEXT("Invalid option index %d"), Index);
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
		for (const FName& ID : NodeInfo->SpeakerIDs) {
			if (!CombinedSpeakerNames.IsEmpty()) {
				CombinedSpeakerNames += TEXT(", ");
			}
			CombinedSpeakerNames += GetSpeakerName(ID);
		}

		DialogueSubsystem->OnDialogueLineRequested.Broadcast(
			FText::FromString(GetDialogueText(NodeInfo->DialogueID)),
			FText::FromString(CombinedSpeakerNames)
			);
		
		AutoSkipDialogueSelector(NodeInfo);
	}
	
	// If the current node is a Branch Node, Display dialogue options if applicable
	else if (CurrentNode == nullptr || CurrentNode->NodeType == EDialogueNodeType::BranchNode) {
		const UDialogueNodeInfoBranch* BranchNodeInfo = Cast<UDialogueNodeInfoBranch>(CurrentNode->NodeInfo);
		
		TArray<FBranchCondition> BranchCache = BranchNodeInfo->BranchOptions;
		
		TArray<FBranchOption> BranchToDisplay;
		for (int i = 0; i < BranchCache.Num(); ++i) {
			bool bIsValid = false; 
			
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
			CurrentBranch.DialogueText = FText::FromString(GetDialogueText(BranchCache[i].DialogueResponse));
			CurrentBranch.Tooltip = FText::FromString(GetDialogueText(BranchCache[i].ConditionTooltip));
			CurrentBranch.bExpressionIsValid = bIsValid;
			BranchToDisplay.Add(CurrentBranch);
		}
		
		DialogueSubsystem->OnBranchOptionsRequested.Broadcast(BranchToDisplay);
	}
	
	// If the current node is an action node, execute the action
	else if (CurrentNode != nullptr && CurrentNode->NodeType == EDialogueNodeType::GameActionNode) {
		const UDialogueNodeInfoGameAction* GameActionNodeInfo = Cast<UDialogueNodeInfoGameAction>(CurrentNode->NodeInfo);

		if (GameActionNodeInfo->GameAction.Num() > 0) {
			FGameActionContext NewContext;
			
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
				
				GameActionSubsystem->ExecuteGameAction(InstancedActions[0], NewContext, OnCompleted);
			}
			else {
				FOnGameActionSequenceCompleted OnSequenceCompleted;
				OnSequenceCompleted.BindDynamic(this, &UDialoguePlayer::OnGameActionFinished);
			
				FOnParallelGameActionCompleted OnParallelCompleted;
				OnParallelCompleted.BindDynamic(this, &UDialoguePlayer::OnGameActionFinished);
				
				switch (GameActionNodeInfo->GameActionExecutionMode) {
					case EGameActionExecutionMode::Sequence:
						GameActionSubsystem->ExecuteGameActionSequence(InstancedActions, NewContext, OnSequenceCompleted);
						break;
					case EGameActionExecutionMode::Parallel:
						GameActionSubsystem->ExecuteParallelGameAction(InstancedActions, NewContext, OnParallelCompleted);
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

	UE_LOG(DialoguePlayerSub, Error, TEXT("No Label found with name %s, Ending Dialogue"), *NodeName.ToString());
	DialogueSubsystem->OnDialogueEnded.Broadcast();
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

void UDialoguePlayer::AutoSkipDialogueSelector(const UDialogueNodeInfoText* NodeInfo)
{
	switch (NodeInfo->SkipDialogue) {
		case ESkipDialogue::NoSkip:
			break;
		case ESkipDialogue::AutoSkipBasedOnText:
			CurrentSkipTime = CalculateSkipTimer(GetDialogueText(NodeInfo->DialogueID));
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

TArray<FText> UDialoguePlayer::StringArrayConverter(TArray<FString> StringArray)
{
	TArray<FText> Result;

	for (FString& String : StringArray) {
		Result.Add(FText::FromString(String));
	}
	
	return Result;
}

FString UDialoguePlayer::GetSpeakerName(const FName& SpeakerName) const
{
	FString NullText;

	if (DialogueSystem == nullptr) {
		return NullText;
	}
	if (SpeakerName.IsNone()) {
		return NullText;
	}
	if (DialogueSystem->SpeakerDataTable == nullptr) {
		return NullText;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DialogueSystem->SpeakerDataTable;
	DataTableRowHandle.RowName = SpeakerName;
	
	FString LocalizedName = UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle);
	
	return LocalizedName;
}

FString UDialoguePlayer::GetDialogueText(const FName& DialogueText) const
{
	FString NullText;

	if (DialogueSystem == nullptr) {
		return NullText;
	}
	if (DialogueText.IsNone()) {
		return NullText;
	}
	if (DialogueSystem->DialogueDataTable == nullptr) {
		return NullText;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DialogueSystem->DialogueDataTable;
	DataTableRowHandle.RowName = DialogueText;
	
	FString LocalizedText = UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle);
	
	return LocalizedText;
}

TArray<FString> UDialoguePlayer::GetDialogueOptionText(const TArray<FName>& DialogueOptionText) const
{
	TArray<FString> NullText;

	if (DialogueSystem == nullptr) {
		return NullText;
	}
	if (DialogueOptionText.Num() <= 0) {
		return NullText;
	}
	if (DialogueSystem->DialogueDataTable == nullptr) {
		return NullText;
	}

	FDataTableRowHandle DataTableRowHandle;
	DataTableRowHandle.DataTable = DialogueSystem->DialogueDataTable;
	TArray<FString> LocalizedText;
	
	for (const FName Text : DialogueOptionText) {
		DataTableRowHandle.RowName = Text;
		LocalizedText.Add(UGSheetLocSystemLibrary::GetLocalizedStringAuto(DataTableRowHandle));
	} 
		
	return LocalizedText;
}
