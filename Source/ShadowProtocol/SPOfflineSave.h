#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SPPlayMode.h"
#include "SPOfflineSave.generated.h"
UCLASS()
class SHADOWPROTOCOL_API USPOfflineSave : public USaveGame
{
 GENERATED_BODY()
public:
 UPROPERTY(BlueprintReadOnly, SaveGame) int32 SaveVersion = 2;
 UPROPERTY(BlueprintReadWrite, SaveGame) int32 UnlockedMission = 1;
 UPROPERTY(BlueprintReadWrite, SaveGame) int32 CompletedMissions = 0;
 UPROPERTY(BlueprintReadWrite, SaveGame) FString LocalAlias = TEXT("Ghost-0001");
 UPROPERTY(BlueprintReadWrite, SaveGame) FSPPlaySelection LastSelection;
 UPROPERTY(BlueprintReadOnly, SaveGame) int32 Coins = 0;
 UPROPERTY(BlueprintReadOnly, SaveGame) int32 Experience = 0;
 UPROPERTY(BlueprintReadOnly, SaveGame) TArray<int32> RewardedMissions;
 UPROPERTY(BlueprintReadOnly, SaveGame) TArray<FName> CollectedPickups;
 UPROPERTY(BlueprintReadOnly, SaveGame) TArray<FName> OwnedCars;
 UPROPERTY(BlueprintReadOnly, SaveGame) FName EquippedCar = NAME_None;
};
