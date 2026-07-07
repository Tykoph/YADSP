// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

/**
 * Defines UI commands available within the Dialogue Graph Editor context.
 */
class FDialogueGraphCommands : public TCommands<FDialogueGraphCommands>
{
public:
	FDialogueGraphCommands();

	TSharedPtr<FUICommandInfo> CreateTextNode;
	TSharedPtr<FUICommandInfo> CreateBranchNode;
	TSharedPtr<FUICommandInfo> CreateGameActionNode;
	TSharedPtr<FUICommandInfo> CreateGoToNode;
	TSharedPtr<FUICommandInfo> CreateLabelNode;
	TSharedPtr<FUICommandInfo> CreateEndNode;
	
	TSharedPtr<FUICommandInfo> DeleteNode;
	
protected:
	/** Registers the graph editor commands with the UI system. */
	virtual void RegisterCommands() override;
};