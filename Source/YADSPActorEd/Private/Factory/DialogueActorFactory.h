#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UObject/Object.h"
#include "DialogueActorFactory.generated.h"

UCLASS()
class YADSPACTORED_API UDialogueActorFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDialogueActorFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* UClass, UObject* InParent, FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
