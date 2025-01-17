#include "UI/DialoguePlayer.h"
#include "UI/DialogueUIController.h"
#include "UI/DialogueOptionController.h"
#include "DialogueSystem.h"
#include "DialogueNodeInfoEnd.h"
#include "DialogueNodeInfoText.h"

#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

DEFINE_LOG_CATEGORY_STATIC(DialoguePlayerSub, Log, All);

void UDialoguePlayer::PlayDialogue(UDialogueSystem* DialogueAsset, APlayerController* PlayerController,
                                   TArray<AActor*> Cameras,
                                   FDialogueEndCallback OnDialogueEnded)
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
			UDialogueOptionController* OptionController = UDialogueOptionController::CreateInstance(
				DialogueUIPtr->GetOwningPlayer());
			OptionController->SetClickHandler(OptionIndex, [this](int OptionIndex)
			{
				ChooseOptionAtIndex(OptionIndex);
			});

			OptionController->ResponseButtonText->SetText(Response);
			UHorizontalBoxSlot* Slot = DialogueUIPtr->ResponseBox->AddChildToHorizontalBox(OptionController);
			Slot->SetPadding(FMargin(10));
			OptionIndex++;
		}

		if (OptionIndex == 1)
		{
			CurrentSkipTime = NodeInfo->SkipAfterSeconds;
			if (NodeInfo->SkipAfterSeconds == -1)
			{
				CurrentSkipTime = CalculateSkipTimer(NodeInfo->DialogueText);
			}
			else if (NodeInfo->SkipAfterSeconds == 0)
			{
				return;
			}

			AutoSkipDialogue(CurrentSkipTime);
		}
	}
	else if (CurrentNodePtr == nullptr || CurrentNodePtr->NodeType == EDialogueNodeType::EndNode)
	{
		DialogueUIPtr->RemoveFromParent();
		DialogueUIPtr = nullptr;

		EDialogueNodeAction Action = EDialogueNodeAction::None;
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

		OnDialogueEndedCallback.Execute(Action, ActionData);
	}
}

float UDialoguePlayer::CalculateSkipTimer(const FText& Text)
{
	int Length = Text.ToString().Len();
	Length = Length / 1600;
	Length = Length * 60;
	Length = Length * 1.2;
	return Length;
}

void UDialoguePlayer::AutoSkipDialogue(float Time)
{
	ChooseOptionAtIndex(0);
}
