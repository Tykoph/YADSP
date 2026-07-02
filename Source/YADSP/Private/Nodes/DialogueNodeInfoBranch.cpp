// Copyright Tom Duby. All Rights Reserved.

#include "Nodes/DialogueNodeInfoBranch.h"

#if WITH_EDITOR
void UDialogueNodeInfoBranch::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UDialogueNodeInfoBranch, BranchOptions)) {
		if (BranchOptions.Num() < 2) {
			BranchOptions.SetNum(2);
		}
	}
}
#endif

TArray<FString> UDialogueNodeInfoBranch::GetTextList() const
{
	TArray<FString> Options;

	if (DialogueSystem && DialogueSystem->DialogueDataTable) {
		TArray<FName> RowNames = DialogueSystem->DialogueDataTable->GetRowNames();

		for (const FName& Name : RowNames) {
			Options.Add(Name.ToString());
		}
	}

	if (Options.Num() == 0) {
		Options.Add(TEXT("No Table Found"));
	}

	return Options;
}

TArray<FString> UDialogueNodeInfoBranch::GetTooltipList() const
{
	TArray<FString> Options;
	
	auto List = GetTextList();
	Options.Reserve(List.Num() + 1);
	
	Options.Add(TEXT("None"));
	for (auto Item : List) {
		Options.Add(Item);
	}
	
	return Options;
}

