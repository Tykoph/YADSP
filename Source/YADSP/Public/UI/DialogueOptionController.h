#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueOptionController.generated.h"

UCLASS()
class YADSP_API UDialogueOptionController : public UUserWidget {
    GENERATED_BODY()

public: // Methods
    UDialogueOptionController(const FObjectInitializer& ObjectInitializer);
    virtual ~UDialogueOptionController() { }

    void SetClickHandler(int Index, std::function<void(int)> ClickHandler);

    UFUNCTION()
    void OnButtonClicked();

public: // Create Method
    static UDialogueOptionController* CreateInstance(APlayerController* PlayerController);

public: // Properties
// ---------- Generated Properties Section ---------- //
//             (Don't modify manually)              //
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ResponseButton = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* ResponseButtonText = nullptr;
// ---------- End Generated Properties Section ---------- //

private: // Fields
    std::function<void(int)> OnClickHandler;
    int ButtonIndex;
};

// ---------- Generated Loader Section ---------- //
//             (Don't modify manually)            //
UCLASS()
class UDialogueOptionLoader : public UObject {
    GENERATED_BODY()
public:
    UDialogueOptionLoader();
    virtual ~UDialogueOptionLoader() { }

public:
    UPROPERTY()
    UClass* WidgetTemplate = nullptr;
    static const inline FString WidgetPath = TEXT("/YADSP/WBP_DialogueOption");
};
// ---------- End Generated Loader Section ---------- //
