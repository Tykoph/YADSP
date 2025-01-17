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
	FDialogueEndCallback OnDialogueEnded)
{
	OnDialogueEndedCallback = OnDialogueEnded;
	UDialogueSystemRuntimeGraph* Graph = DialogueAsset->Graph;

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

	UDialogueRuntimeGraphPin* SelectedPin = CurrentNodePtr->OutputPins[Index];
	if (SelectedPin->Connection != nullptr)	{
		CurrentNodePtr = SelectedPin->Connection->Parent;
	} else {
		// No Connection, this is an end node
		CurrentNodePtr = nullptr;
	}

	if (CurrentNodePtr != nullptr && CurrentNodePtr->NodeType == EDialogueNodeType::TextNode) {
		UDialogueNodeInfoText* NodeInfo = Cast<UDialogueNodeInfoText>(CurrentNodePtr->NodeInfo);
		DialogueUIPtr->DialogueText->SetText(NodeInfo->DialogueText);
		DialogueUIPtr->SpeakerName->SetText(NodeInfo->Speaker);

		DialogueUIPtr->ResponseBox->ClearChildren();
		int OptionIndex = 0;
		for (FText Response : NodeInfo->DialogueResponses)
		{
			UDialogueOptionController* OptionController = UDialogueOptionController::CreateInstance(DialogueUIPtr->GetOwningPlayer());
			OptionController->SetClickHandler(OptionIndex, [this] (int OptionIndex){
				ChooseOptionAtIndex(OptionIndex);
			});

			OptionController->ResponseButtonText->SetText(Response);
			UHorizontalBoxSlot* Slot = DialogueUIPtr->ResponseBox->AddChildToHorizontalBox(OptionController);
			Slot->SetPadding(FMargin(10));
			OptionIndex++;
		}

	} else if (CurrentNodePtr == nullptr || CurrentNodePtr->NodeType == EDialogueNodeType::EndNode) {
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

		OnDialogueEndedCallback.Execute(Action, ActionData);
	}
}
