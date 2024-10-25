#include "MoreLightingOptionsFunctionLibrary.h"
#include "MoreLightingOptionsModule.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Buildables/FGBuildableWidgetSign.h"
#include "MoreLightingOptions_ConfigStruct.h"
#include "TimerManager.h"

TMap<uint32, FTimerHandle> SignTimerHandles;

void UMoreLightingOptionsFunctionLibrary::RevertEmissiveValue(AFGBuildableWidgetSign* Sign, float OriginalEmissiveValue)
{
    if (IsValid(Sign))
    {
        Sign->mEmissive = OriginalEmissiveValue;
        // UE_LOG(LogTemp, Warning, TEXT("MLO: Reverted mEmissive back to %f for Sign %u"), OriginalEmissiveValue, Sign->mCachedGUID);

        // Trigger another update to reflect the original emissive value
        FPrefabSignData PrefabSignData;
        Sign->GetSignPrefabData(PrefabSignData);
        FPrefabSignData MutablePrefabSignData = PrefabSignData;
        Sign->UpdateSignElements(MutablePrefabSignData);

        // Clear the timer
        uint32 SignGUID = Sign->mCachedGUID;
        if (SignTimerHandles.Contains(SignGUID))
        {
            FTimerHandle& TimerHandle = SignTimerHandles[SignGUID];
            if (GEngine && GEngine->GetWorld())
            {
                GEngine->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            }
            SignTimerHandles.Remove(SignGUID);
        }
    }
}

void UMoreLightingOptionsFunctionLibrary::UpdateAllSigns(bool bIsFast)
{
//    UE_LOG(LogTemp, Warning, TEXT("MLO: UpdateAllSigns - Starting sign update process"));

    if (!GEngine)
    {
        UE_LOG(LogTemp, Error, TEXT("MLO: UpdateAllSigns - GEngine is null"));
        return;
    }

    UWorld* World = nullptr;
    for (const FWorldContext& Context : GEngine->GetWorldContexts())
    {
        if (Context.World() && Context.WorldType == EWorldType::Game)
        {
            World = Context.World();
            break;
        }
    }

    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("MLO: UpdateAllSigns - World is null"));
        return;
    }

    // Fast or slow update
    float RevertDelay = bIsFast ? 0.02f : 1.0f;

    // Iterate all signs ... maybe we can stagger this?
    for (TActorIterator<AFGBuildableWidgetSign> It(World); It; ++It)
    {
        AFGBuildableWidgetSign* Sign = *It;

        if (!IsValid(Sign))
        {
            UE_LOG(LogTemp, Warning, TEXT("MLO: UpdateAllSigns - Skipping sign"));
            continue;
        }

        uint32 SignGUID = Sign->mCachedGUID;
        // Clear existing timer
        if (SignTimerHandles.Contains(SignGUID))
        {
            FTimerHandle& ExistingTimerHandle = SignTimerHandles[SignGUID];
            World->GetTimerManager().ClearTimer(ExistingTimerHandle);
        }

        float OriginalEmissiveValue = Sign->mEmissive;
        // UE_LOG(LogTemp, Warning, TEXT("MLO: Sign GUID: %u, Current mEmissive: %f"), Sign->mCachedGUID, OriginalEmissiveValue);
        float TempEmissiveValue = OriginalEmissiveValue + 0.01f; // Minimal change
        Sign->mEmissive = TempEmissiveValue;
        // UE_LOG(LogTemp, Warning, TEXT("MLO: Sign GUID: %u, Temp mEmissive: %f"), Sign->mCachedGUID, TempEmissiveValue);

        // Force update signs
        FPrefabSignData PrefabSignData;
        Sign->GetSignPrefabData(PrefabSignData);
        FPrefabSignData MutablePrefabSignData = PrefabSignData;
        Sign->UpdateSignElements(MutablePrefabSignData);

        // timer to revert the emissive value
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([Sign, OriginalEmissiveValue]()
        {
            RevertEmissiveValue(Sign, OriginalEmissiveValue);
        }), RevertDelay, false);
        SignTimerHandles.Add(SignGUID, TimerHandle);
    }
}

void UMoreLightingOptionsFunctionLibrary::ClearAllSignTimers()
{
    UWorld* World = GEngine->GetWorld();
    if (!World) return;

    for (auto& Elem : SignTimerHandles)
    {
        FTimerHandle& TimerHandle = Elem.Value;
        World->GetTimerManager().ClearTimer(TimerHandle);
    }
    SignTimerHandles.Empty();
}

void UMoreLightingOptionsFunctionLibrary::UpdateAllSignsFast()
{
    UpdateAllSigns(true);
}

void UMoreLightingOptionsFunctionLibrary::UpdateAllSignsSlow()
{
    UpdateAllSigns(false);
}
