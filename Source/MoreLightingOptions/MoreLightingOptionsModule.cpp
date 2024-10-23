#include "MoreLightingOptionsModule.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "Patching/NativeHookManager.h"


// For Signs

class HFGBuildableWidgetSign
{
public:
    static void Hook_GetAdjustedEmissiveValue(TCallScope<float(__cdecl*)(const AFGBuildableWidgetSign*, int32)>& scope, const AFGBuildableWidgetSign* self, int32 Level)
    {
        if (!self) {
            return;
        }
        float NewEmissiveValue = 0.0f;
        switch (Level)
        {
        case 0:
            NewEmissiveValue = 0.0f;
            break;
        case 1:
            // Originally 1.0
            NewEmissiveValue = 2.0f;
            break;
        case 2:
            // Originally 5.0
            NewEmissiveValue = 10.0f;
            break;
        case 3:
            // Originally 10.0
            NewEmissiveValue = 50.0f;
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
