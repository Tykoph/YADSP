// Copyright Tom Duby. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Defines the unified logging category spanning all YADSP internal logic.
YADSP_API DECLARE_LOG_CATEGORY_EXTERN(LogYADSP, Log, All);

/**
 * Primary lifecycle manager tracking execution states spanning the YADSP plugin module.
 */
class FYADSPModule : public IModuleInterface
{

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
