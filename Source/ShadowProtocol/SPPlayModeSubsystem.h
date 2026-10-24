#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPPlayMode.h"
#include "SPPlayModeSubsystem.generated.h"
UCLASS()
class SHADOWPROTOCOL_API USPPlayModeSubsystem : public UGameInstanceSubsystem
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintCallable, Category="Play") bool SelectMode(FSPPlaySelection Requested);
 UFUNCTION(BlueprintPure, Category="Play") FSPPlaySelection GetSelection() const { return Selection; }
 UFUNCTION(BlueprintPure, Category="Play") bool RequiresNetwork() const { return Selection.Connectivity == ESPConnectivity::Online; }
private:
 UPROPERTY() FSPPlaySelection Selection;
};
