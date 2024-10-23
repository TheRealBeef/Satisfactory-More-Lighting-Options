#include "MoreLightingOptionsModule.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Patching/NativeHookManager.h"
#include "MoreLightingOptions_ConfigStruct.h"

// For Signs
class HFGBuildableWidgetSign
{
public:
    static void Hook_GetAdjustedEmissiveValue(TCallScope<float(__cdecl*)(const AFGBuildableWidgetSign*, int32)>& scope, const AFGBuildableWidgetSign* self, int32 Level)
    {
        if (!self) {
            return;
        }

        FMoreLightingOptions_ConfigStruct config = FMoreLightingOptions_ConfigStruct::GetActiveConfig(const_cast<AFGBuildableWidgetSign*>(self));

        float NewEmissiveValue = 0.0f;

        switch (Level)
        {
        case 0:
            NewEmissiveValue = config.SignBrightness0;
            break;
        case 1:
            // Originally 1.0
            NewEmissiveValue = config.SignBrightness1;
            break;
        case 2:
            // Originally 5.0
            NewEmissiveValue = config.SignBrightness2;
            break;
        case 3:
            // Originally 10.0
            NewEmissiveValue = config.SignBrightness3;
            break;
        }
        scope.Override(NewEmissiveValue);
    }

    static void RegisterHooks()
    {
        SUBSCRIBE_UOBJECT_METHOD(AFGBuildableWidgetSign, GetAdjustedEmissiveValue, &HFGBuildableWidgetSign::Hook_GetAdjustedEmissiveValue);
    }
};

void FMoreLightingOptionsModule::StartupModule()
{
    if (!WITH_EDITOR)
    {
        HFGBuildableWidgetSign::RegisterHooks();
    }
}

void FMoreLightingOptionsModule::ShutdownModule()
{
}

IMPLEMENT_GAME_MODULE(FMoreLightingOptionsModule, MoreLightingOptions);
