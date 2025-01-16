#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueUIController.generated.h"

UCLASS()
class YADSP_API UDialogueUIController : public UUserWidget {
    GENERATED_BODY()

public: // Methods
    UDialogueUIController(const FObjectInitializer& objectInitializer);
    virtual ~UDialogueUIController() { }

public: // Create Method
    static UDialogueUIController* CreateInstance(APlayerController* PlayerController);

public: // Properties
// ---------- Generated Properties Section ---------- //
//             (Don't modify manually)              //
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* SpeakerName = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* DialogueText = nullptr;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UHorizontalBox* ResponseBox = nullptr;
// ---------- End Generated Properties Section ---------- //
};

// ---------- Generated Loader Section ---------- //
//             (Don't modify manually)            //
UCLASS()
class UDialogueUILoader : public UObject {
    GENERATED_BODY()
public:
    UDialogueUILoader();
    virtual ~UDialogueUILoader() { }

public:
    UPROPERTY()
    UClass* WidgetTemplate = nullptr;
    static const inline FString WidgetPath = TEXT("/YADSP/WBP_ExampleDialogueUI");
};
// ---------- End Generated Loader Section ---------- //
