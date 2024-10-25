#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "MoreLightingOptions_ConfigStruct.generated.h"

/* Struct generated from Mod Configuration Asset '/MoreLightingOptions/MoreLightingOptions_Config' */
USTRUCT(BlueprintType)
struct FMoreLightingOptions_ConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    float SignBrightness0{};

    UPROPERTY(BlueprintReadWrite)
    float SignBrightness1{};

    UPROPERTY(BlueprintReadWrite)
    float SignBrightness2{};

    UPROPERTY(BlueprintReadWrite)
    float SignBrightness3{};

    UPROPERTY(BlueprintReadWrite)
    bool FastSignUpdate{};

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FMoreLightingOptions_ConfigStruct GetActiveConfig(UObject* WorldContext) {
        FMoreLightingOptions_ConfigStruct ConfigStruct{};
        FConfigId ConfigId{"MoreLightingOptions", ""};
        if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull)) {
            UConfigManager* ConfigManager = World->GetGameInstance()->GetSubsystem<UConfigManager>();
            ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FMoreLightingOptions_ConfigStruct::StaticStruct(), &ConfigStruct});
        }
        return ConfigStruct;
    }
};

