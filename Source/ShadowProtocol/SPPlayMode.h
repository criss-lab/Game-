#pragma once
#include "CoreMinimal.h"
#include "SPPlayMode.generated.h"
UENUM(BlueprintType)
enum class ESPConnectivity : uint8 { Offline, Online };
UENUM(BlueprintType)
enum class ESPPlayMode : uint8 { Story, SoloVsAI, PrivateDuel, CopsAndThieves };
USTRUCT(BlueprintType)
struct FSPPlaySelection
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite) ESPConnectivity Connectivity = ESPConnectivity::Offline;
 UPROPERTY(EditAnywhere, BlueprintReadWrite) ESPPlayMode Mode = ESPPlayMode::Story;
 bool IsValid() const
 {
  return Connectivity == ESPConnectivity::Online || Mode == ESPPlayMode::Story || Mode == ESPPlayMode::SoloVsAI || Mode == ESPPlayMode::CopsAndThieves;
 }
};
