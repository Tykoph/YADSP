// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FDialogueGraphCommands : public TCommands<FDialogueGraphCommands>
{
public:
	FDialogueGraphCommands();
	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> CreateTextNode;
	TSharedPtr<FUICommandInfo> CreateBranchNode;
	TSharedPtr<FUICommandInfo> CreateGameActionNode;
	TSharedPtr<FUICommandInfo> CreateGoToNode;
	TSharedPtr<FUICommandInfo> CreateLabelNode;
	TSharedPtr<FUICommandInfo> CreateEndNode;
	
	TSharedPtr<FUICommandInfo> DeleteNode;
};