#pragma once
#include "CoreMinimal.h"
#include "SPProgressionTypes.generated.h"
UENUM(BlueprintType)
enum class ESPRank : uint8 { Recruit, Operative, Specialist, Elite, Legend };
USTRUCT(BlueprintType)
struct FSPCarOffer
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Id = NAME_None;
 UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0")) int32 Price = 0;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1")) int32 RequiredMission = 1;
};
USTRUCT(BlueprintType)
struct FSPMissionReward
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="1")) int32 Mission = 1;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0")) int32 Coins = 100;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0")) int32 XP = 100;
};
