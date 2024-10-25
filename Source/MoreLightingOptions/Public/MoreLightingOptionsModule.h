#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"

class FMoreLightingOptionsModule : public IModuleInterface {
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

// Something to store the GUIDs ... we'll use this later
//extern TMap<uint32, int32> SignEmissiveLevels;

// Something to store the GUIDs ... we'll use this later
//extern TMap<uint32, int32> SignEmissiveLevels;