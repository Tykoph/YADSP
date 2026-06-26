// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSubsystem.h"
#include "Blueprint/UserWidget.h"
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
	class UButton* ResponseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class URichTextBlock* ResponseButtonText = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = true;
	
	UPROPERTY(BlueprintReadOnly)
	FText OptionTooltip;
	
private:
	int ButtonIndex;
	
	UPROPERTY()
	UDialogueSubsystem* DialogueSubsystem;
};
