#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewQuestController.Generated.h"

UCLASS()
class YADSP_API UNewQuestController : public UUserWidget {
    GENERATED_BODY()

public: // Methods
    UNewQuestController(const FObjectInitializer& objectInitializer);
    virtual ~UNewQuestController() { }

public: // Create Method
    static UNewQuestController* CreateInstance(APlayerController* PlayerController);

public: // Properties
// ---------- Generated Properties Section ---------- //
//             (Don't modify manually)              //
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* QuestText = nullptr;
// ---------- End Generated Properties Section ---------- //
};

// ---------- Generated Loader Section ---------- //
//             (Don't modify manually)            //
UCLASS()
class UNewQuestLoader : public UObject {
    GENERATED_BODY()
public:
    UNewQuestLoader();
    virtual ~UNewQuestLoader() { }

public:
    UPROPERTY()
    UClass* WidgetTemplate = nullptr;
    static const inline FString WidgetPath = TEXT("/YADSP/WBP_NewQuest");
};
// ---------- End Generated Loader Section ---------- //
