#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "MoreLightingOptionsFunctionLibrary.generated.h"

UCLASS()
class MORELIGHTINGOPTIONS_API UMoreLightingOptionsFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Blueprint exposed functions
    UFUNCTION(BlueprintCallable, Category = "MoreLightingOptions")
    static void UpdateAllSignsFast();
    UFUNCTION(BlueprintCallable, Category = "MoreLightingOptions")
    static void UpdateAllSignsSlow();
    UFUNCTION(BlueprintCallable, Category = "MoreLightingOptions")
    static void ClearAllSignTimers();

    static void UpdateAllSigns(bool bIsFast);
    static void RevertEmissiveValue(AFGBuildableWidgetSign* Sign, float OriginalEmissiveValue);


    ////////////////////////
    // For Later Reference
    ////////////////////////
    //    // get the Cached GUID
    //    static uint32 GetSignCachedGUID(const AFGBuildableWidgetSign* Sign);
};
