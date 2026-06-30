// Copyright Tom Duby. All Rights Reserved.

#include "DialogueGraphCommands.h"

#define LOCTEXT_NAMESPACE "DialogueGraphCommands"

FDialogueGraphCommands::FDialogueGraphCommands()
	: TCommands<FDialogueGraphCommands>(TEXT("DialogueGraph"), LOCTEXT("DialogueGraph", "Dialogue Graph"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FDialogueGraphCommands::RegisterCommands()
{
	UI_COMMAND(CreateTextNode, "Create Text Node", "Creates a new Text node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateBranchNode, "Create Branch Node", "Creates a new Branch node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateGameActionNode, "Create GameAction Node", "Creates a new GameAction node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateGoToNode, "Create GoTo Node", "Creates a new GoTo node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateLabelNode, "Create Label Node", "Creates a new Label node", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CreateEndNode, "Create End Node", "Creates a new End node", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(DeleteNode, "Delete Node", "Deletes the selected nodes", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE