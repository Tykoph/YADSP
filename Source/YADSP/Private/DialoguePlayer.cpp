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
	// Save the callback so we can call it when the dialogue ends
	OnDialogueEndedCallback = OnDialogueEnded;

	// Get the dialogue graph
	UDialogueSystemRuntimeGraph* Graph = DialogueAsset->Graph;

	// Save the dialogue asset and player controller
	DialogueAssetPtr = DialogueAsset;
	DialogueAssetPtr->CameraActors = Cameras;
	DialogueAssetPtr->DefaultCamera = PlayerController->PlayerCameraManager->GetViewTarget();

	PlayerControllerPtr = PlayerController;

	// Disable player input
	// TODO : Add an option to not disable player input (dialogue during gameplay)
	PlayerControllerPtr->SetShowMouseCursor(true);
	PlayerControllerPtr->SetIgnoreLookInput(true);
	PlayerControllerPtr->SetIgnoreMoveInput(true);
	PlayerControllerPtr->SetInputMode(FInputModeUIOnly());

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

	// Display Dialogue UI
	DialogueUIPtr = UDialogueUIController::CreateInstance(PlayerController);
	DialogueUIPtr->AddToViewport();

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

	APlayerCameraManager* CameraManager = DialogueUIPtr->GetOwningPlayer()->PlayerCameraManager;

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

		// Set the dialogue text
		DialogueUIPtr->DialogueText->SetText(NodeInfo->DialogueText);

		// Set the speaker name
		DialogueUIPtr->SpeakerName->SetText(NodeInfo->GetSpeakerName(NodeInfo->Speaker));

		// Check if the dialogue text is too long to fit in the dialogue box
		DialogueUIPtr->IsTextWrapping(DialogueUIPtr->DialogueText, NodeInfo->DialogueText.ToString());

		// Set the camera target
		if (NodeInfo->GetCameraIndex() == -1) {
			CameraManager->SetViewTarget(DialogueAssetPtr->DefaultCamera);
		}
		else if (NodeInfo->GetCameraIndex() < DialogueAssetPtr->CameraActors.Num()) {
			CameraManager->SetViewTarget(DialogueAssetPtr->CameraActors[NodeInfo->GetCameraIndex()]);
		}

		// Add the response buttons to the dialogue box
		DialogueUIPtr->ResponseBox->ClearChildren();
		int OptionIndex = 0;
		for (FText Response : NodeInfo->DialogueResponses) {
			// Create a new response button
			UDialogueOptionController* OptionController = UDialogueOptionController::CreateInstance(DialogueUIPtr->GetOwningPlayer());

			// Set the click handler to navigate to the selected node
			OptionController->SetClickHandler(OptionIndex, [this](int OptionIndex)
			{
				ChooseOptionAtIndex(OptionIndex);
			});

			// Set the response text
			OptionController->ResponseButtonText->SetText(Response);

			// Add the response button to the dialogue box
			UHorizontalBoxSlot* Slot = DialogueUIPtr->ResponseBox->AddChildToHorizontalBox(OptionController);
			Slot->SetPadding(FMargin(10));
			OptionIndex++;
		}

		// If there is only one response, auto skip the dialogue
		if (NodeInfo->DialogueResponses.Num() == 1) {
			// Check the skip dialogue type
			switch (NodeInfo->SkipDialogue) {
			case ESkipDialogue::NoSkip:
				break;
			case ESkipDialogue::AutoSkipBasedOnText:
				// Calculate the skip time based on the text length
				CurrentSkipTime = CalculateSkipTimer(NodeInfo->DialogueText);
				AutoSkipDialogue(CurrentSkipTime);
				break;
			case ESkipDialogue::AutoSkipAfterSound:
				// Play the sound and skip after it finishes
				if (NodeInfo->DialogueSound == nullptr) break;
				CurrentSkipTime = NodeInfo->DialogueSound->GetDuration();
				UGameplayStatics::PlaySound2D(GetWorld(), NodeInfo->DialogueSound);
				AutoSkipDialogue(CurrentSkipTime);
				break;
			case ESkipDialogue::AutoSkipAfterTime:
				// Skip after the specified time
				CurrentSkipTime = NodeInfo->SkipAfterSeconds;
				AutoSkipDialogue(CurrentSkipTime);
				break;
			}
		}
	}

	// If the current node is an action node, execute the action
	else if (CurrentNodePtr != nullptr && CurrentNodePtr->NodeType == EDialogueNodeType::ActionNode) {
		FString ActionData = TEXT("");
		UDialogueNodeInfoAction* ActionNodeInfo = Cast<UDialogueNodeInfoAction>(CurrentNodePtr->NodeInfo);
		EDialogueAction Action = ActionNodeInfo->Action;
		ActionData = ActionNodeInfo->ActionData;

		OnDialogueEndedCallback.Execute(Action, ActionData);
		ChooseOptionAtIndex(0);
	}

	// If the current node is an end node, end the dialogue
	else if (CurrentNodePtr == nullptr || CurrentNodePtr->NodeType == EDialogueNodeType::EndNode) {
		DialogueUIPtr->RemoveFromParent();
		DialogueUIPtr = nullptr;

		EDialogueAction Action = EDialogueAction::None;
		FString ActionData = TEXT("");
		if (CurrentNodePtr != nullptr) {
			UDialogueNodeInfoEnd* EndNodeInfo = Cast<UDialogueNodeInfoEnd>(CurrentNodePtr->NodeInfo);
			Action = EndNodeInfo->Action;
			ActionData = EndNodeInfo->ActionData;
		}

		// Restore player input settings and set the camera target after dialogue ends
		CameraManager->SetViewTarget(DialogueAssetPtr->DefaultCamera);
		PlayerControllerPtr->SetShowMouseCursor(false);
		PlayerControllerPtr->SetIgnoreLookInput(false);
		PlayerControllerPtr->SetIgnoreMoveInput(false);

		OnDialogueEndedCallback.Execute(Action, ActionData);
	}
}

float UDialoguePlayer::CalculateSkipTimer(const FText& Text)
{
	// Convert the FText to an FString to get its length
	FString Buffer = Text.ToString();

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
