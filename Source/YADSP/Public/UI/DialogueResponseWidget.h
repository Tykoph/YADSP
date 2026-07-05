// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "DialogueResponseWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class YADSP_API UDialogueResponseWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void SetDialogueOption(const FText& InText, const int InIndex);

	UFUNCTION()
	void OnButtonClicked();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<UButton> ResponseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category="YADSP")
	TObjectPtr<URichTextBlock> ResponseButtonText = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	bool bIsValid = true;
	
	UPROPERTY(BlueprintReadOnly, Category="YADSP")
	FText OptionTooltip;
	
private:
	int ButtonIndex;
	
	UPROPERTY()
	TObjectPtr<UDialogueSubsystem> DialogueSubsystem;
};
