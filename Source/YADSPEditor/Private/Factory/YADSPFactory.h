#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "YADSPFactory.generated.h"

/**
 *
 */
UCLASS()
class YADSPEDITOR_API UYADSPFactory : public UFactory
{
	GENERATED_BODY()

public:
	UYADSPFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
};
