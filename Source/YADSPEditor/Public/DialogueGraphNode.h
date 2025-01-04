#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogueGraphNode.generated.h"

UCLASS()
class UDialogueGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public: // UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override { return FColor::FromHex(TEXT("007FFF")); }
	virtual bool CanUserDeleteNode() const override {return true;}
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

public:
	UEdGraphPin* CreateDialoguePin(EEdGraphPinDirection Dir, FName Name);
	void SyncWithNodeResponse();

	void SetNodeInfo(class UDialogueSystemNodeInfo* NodeInfo) {NodeInfoPtr = NodeInfo;}
	class UDialogueSystemNodeInfo* GetNodeInfo() const { return NodeInfoPtr;}



protected:
	UPROPERTY()
	class UDialogueSystemNodeInfo* NodeInfoPtr = nullptr;
};
