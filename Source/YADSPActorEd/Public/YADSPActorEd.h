#pragma once

#include "CoreMinimal.h"

class FYADSPActorEdModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
