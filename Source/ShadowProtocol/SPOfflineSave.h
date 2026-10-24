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
 UPROPERTY(BlueprintReadOnly, SaveGame) int32 SaveVersion = 1;
 UPROPERTY(BlueprintReadWrite, SaveGame) int32 UnlockedMission = 1;
 UPROPERTY(BlueprintReadWrite, SaveGame) int32 CompletedMissions = 0;
 UPROPERTY(BlueprintReadWrite, SaveGame) FSPPlaySelection LastSelection;
};
