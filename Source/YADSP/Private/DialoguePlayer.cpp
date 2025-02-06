// Copyright 2025 Tom Duby. All Rights Reserved.

#include "DialoguePlayer.h"
#include "DialogueNodeInfoAction.h"
#include "UI/DialogueUIController.h"
#include "UI/DialogueOptionController.h"
#include "DialogueSystem.h"
#include "DialogueNodeInfoEnd.h"
#include "DialogueNodeInfoText.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(DialoguePlayerSub, Log, All);

void UDialoguePlayer::PlayDialogue(UDialogueSystem* DialogueAsset, APlayerController* PlayerController, TArray<ADialogueCamera*> Cameras, FDialogueEndCallback OnDialogueEnded)
{
	OnDialogueEndedCallback = OnDialogueEnded;
	UDialogueSystemRuntimeGraph* Graph = DialogueAsset->Graph;

	DialogueAssetPtr = DialogueAsset;
	DialogueAssetPtr->CameraActors = Cameras;
	DialogueAssetPtr->DefaultCamera = PlayerController->PlayerCameraManager->GetViewTarget();

	PlayerControllerPtr = PlayerController;
	PlayerControllerPtr->SetShowMouseCursor(true);
	PlayerControllerPtr->SetIgnoreLookInput(true);
	PlayerControllerPtr->SetIgnoreMoveInput(true);
	PlayerControllerPtr->SetInputMode(FInputModeUIOnly());

	// Get the start node
	for (UDialogueRuntimeGraphNode* Node : Graph->Nodes)
	{
		if (Node->NodeType == EDialogueNodeType::StartNode)
		{
			CurrentNodePtr = Node;
			break;
		}
	}

	if (CurrentNodePtr == nullptr)
	{
		UE_LOG(DialoguePlayerSub, Error, TEXT("No start node found in the dialogue graph"));
		return;
	}

	// Display Dialogue UI
	DialogueUIPtr = UDialogueUIController::CreateInstance(PlayerController);
	DialogueUIPtr->AddToViewport();

	// Play the first node
	ChooseOptionAtIndex(0);
}

void UDialoguePlayer::ChooseOptionAtIndex(int Index)
{
	if (Index >= CurrentNodePtr->OutputPins.Num() || Index < 0)
	{
		UE_LOG(DialoguePlayerSub, Error, TEXT("Invalid option index %d"), Index);
		return;
	}

	APlayerCameraManager* CameraManager = DialogueUIPtr->GetOwningPlayer()->PlayerCameraManager;

	UDialogueRuntimeGraphPin* SelectedPin = CurrentNodePtr->OutputPins[Index];
	if (SelectedPin->Connection != nullptr)
	{
		CurrentNodePtr = SelectedPin->Connection->Parent;
	}
	else
	{
		// No Connection, this is an end node
		CurrentNodePtr = nullptr;
	}

	if (CurrentNodePtr != nullptr && CurrentNodePtr->NodeType == EDialogueNodeType::TextNode)
	{
		UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(CurrentNodePtr->NodeInfo);
		DialogueUIPtr->DialogueText->SetText(NodeInfo->DialogueText);
		DialogueUIPtr->SpeakerName->SetText(NodeInfo->Speaker);
		DialogueUIPtr->IsTextWrapping(DialogueUIPtr->DialogueText, NodeInfo->DialogueText.ToString());

		if (NodeInfo->CameraIndex == -1)
		{
			CameraManager->SetViewTarget(DialogueAssetPtr->DefaultCamera);
		}
		else if (NodeInfo->CameraIndex < DialogueAssetPtr->CameraActors.Num())
		{
			CameraManager->SetViewTarget(DialogueAssetPtr->CameraActors[NodeInfo->CameraIndex]);
		}

		DialogueUIPtr->ResponseBox->ClearChildren();
		int OptionIndex = 0;
		for (FText Response : NodeInfo->DialogueResponses)
		{
			UDialogueOptionController* OptionController = UDialogueOptionController::CreateInstance(DialogueUIPtr->GetOwningPlayer());
			OptionController->SetClickHandler(OptionIndex, [this](int OptionIndex)
			{
				ChooseOptionAtIndex(OptionIndex);
			});

			OptionController->ResponseButtonText->SetText(Response);
			UHorizontalBoxSlot* Slot = DialogueUIPtr->ResponseBox->AddChildToHorizontalBox(OptionController);
			Slot->SetPadding(FMargin(10));
			OptionIndex++;
		}

		if (NodeInfo->DialogueResponses.Num() == 1)
		{
			UE_LOG(DialoguePlayerSub, Log, TEXT("Auto Skip"));

			switch (NodeInfo->SkipDialogue)
			{
				case ESkipDialogue::NoSkip:
					break;
				case ESkipDialogue::AutoSkipBasedOnText:
					CurrentSkipTime = CalculateSkipTimer(NodeInfo->DialogueText);
					AutoSkipDialogue(CurrentSkipTime);
					break;
				case ESkipDialogue::AutoSkipAfterSound:
					if (NodeInfo->DialogueSound == nullptr) break;
					CurrentSkipTime = NodeInfo->DialogueSound->GetDuration();
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
	else if (CurrentNodePtr != nullptr && CurrentNodePtr->NodeType == EDialogueNodeType::ActionNode)
	{
		FString ActionData = TEXT("");
		UDialogueNodeInfoAction* ActionNodeInfo = Cast<UDialogueNodeInfoAction>(CurrentNodePtr->NodeInfo);
		EDialogueAction Action = ActionNodeInfo->Action;
		ActionData = ActionNodeInfo->ActionData;

		OnDialogueEndedCallback.Execute(Action, ActionData);
		ChooseOptionAtIndex(0);
	}
	else if (CurrentNodePtr == nullptr || CurrentNodePtr->NodeType == EDialogueNodeType::EndNode)
	{
		DialogueUIPtr->RemoveFromParent();
		DialogueUIPtr = nullptr;

		EDialogueAction Action = EDialogueAction::None;
		FString ActionData = TEXT("");
		if (CurrentNodePtr != nullptr)
		{
			UDialogueNodeInfoEnd* EndNodeInfo = Cast<UDialogueNodeInfoEnd>(CurrentNodePtr->NodeInfo);
			Action = EndNodeInfo->Action;
			ActionData = EndNodeInfo->ActionData;
		}

		CameraManager->SetViewTarget(DialogueAssetPtr->DefaultCamera);
		PlayerControllerPtr->SetShowMouseCursor(false);
		PlayerControllerPtr->SetIgnoreLookInput(false);
		PlayerControllerPtr->SetIgnoreMoveInput(false);
		PlayerControllerPtr->SetInputMode(FInputModeGameOnly());

		OnDialogueEndedCallback.Execute(Action, ActionData);
	}
}

float UDialoguePlayer::CalculateSkipTimer(const FText& Text)
{
	FString Buffer = Text.ToString();
	float Length = Buffer.Len();
	Length = Length / 15;
	Length = Length * 1.2;
	return Length;
}

void UDialoguePlayer::AutoSkipDialogue(float Time)
{
	GetWorldTimerManager().SetTimer(AutoSkipTimerHandle, [this]()
	{
		ChooseOptionAtIndex(0);
	}, Time, false);
}
