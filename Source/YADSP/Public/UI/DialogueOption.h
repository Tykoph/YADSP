// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "DialogueOption.generated.h"

UCLASS()
class YADSP_API UDialogueOption : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void SetDialogueOption(const FText& Text, const int Index);

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ResponseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<URichTextBlock> ResponseButtonText = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = true;
	
	UPROPERTY(BlueprintReadOnly)
	FText OptionTooltip;
	
private:
	int ButtonIndex;
	
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;
};
