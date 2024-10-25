#include "MoreLightingOptionsModule.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Patching/NativeHookManager.h"
#include "MoreLightingOptions_ConfigStruct.h"
#include "MoreLightingOptionsFunctionLibrary.h"

////////////////////////
// For Later Reference
////////////////////////
// Global map for storing sign emissive levels, we'll use this later
//TMap<uint32, int32> SignEmissiveLevels;

class HFGBuildableWidgetSign
{
public:
    static void Hook_GetAdjustedEmissiveValue(TCallScope<float(__cdecl*)(const AFGBuildableWidgetSign*, int32)>& scope, const AFGBuildableWidgetSign* self, int32 Level)
    {
        if (!self) {
            UE_LOG(LogTemp, Warning, TEXT("MLO: 'self' is null, skipping"));
            return;
        }

        ////////////////////////
        // For Later Reference
        ////////////////////////
        //        // Get the GUID
        //        uint32 SignGUID = UMoreLightingOptionsFunctionLibrary::GetSignCachedGUID(self);
        //        // Store level for GUID
        //        SignEmissiveLevels.Add(SignGUID, Level);
        //        UE_LOG(LogTemp, Warning, TEXT("MLO: Sign GUID: %u, Level: %d"), SignGUID, Level);

        FMoreLightingOptions_ConfigStruct config = FMoreLightingOptions_ConfigStruct::GetActiveConfig(const_cast<AFGBuildableWidgetSign*>(self));
        float NewEmissiveValue = 0.0f;

        switch (Level)
        {
        case 0:
            NewEmissiveValue = config.SignBrightness0;
            break;
        case 1:
            NewEmissiveValue = config.SignBrightness1;
            break;
        case 2:
            NewEmissiveValue = config.SignBrightness2;
            break;
        case 3:
            NewEmissiveValue = config.SignBrightness3;
            break;
        }

        // UE_LOG(LogTemp, Warning, TEXT("MLO: Adjusting Emissive Value for Level %d: %f"), Level, NewEmissiveValue);
        scope.Override(NewEmissiveValue);
    }

    static void RegisterHooks()
    {
        UE_LOG(LogTemp, Warning, TEXT("MLO: Registering hooks for AFGBuildableWidgetSign"));
        SUBSCRIBE_UOBJECT_METHOD(AFGBuildableWidgetSign, GetAdjustedEmissiveValue, &HFGBuildableWidgetSign::Hook_GetAdjustedEmissiveValue);
    }
};

void FMoreLightingOptionsModule::StartupModule()
{
    if (!WITH_EDITOR)
    {
        UE_LOG(LogTemp, Warning, TEXT("MLO: FMoreLightingOptionsModule Registering hooks"));
        HFGBuildableWidgetSign::RegisterHooks();
    }
}

void FMoreLightingOptionsModule::ShutdownModule()
{
     UE_LOG(LogTemp, Warning, TEXT("MLO: FMoreLightingOptionsModule ShutdownModule called"));
}

IMPLEMENT_GAME_MODULE(FMoreLightingOptionsModule, MoreLightingOptions);
