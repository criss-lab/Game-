#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SPPlayerState.h"
#include "SPMatchGameMode.generated.h"
UENUM(BlueprintType)
enum class ESPMatchMode : uint8 { SoloVsAI, PrivateDuel, CopsAndThieves };
UCLASS()
class SHADOWPROTOCOL_API ASPMatchGameMode : public AGameModeBase
{
 GENERATED_BODY()
public:
 ASPMatchGameMode();
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Match") ESPMatchMode MatchMode = ESPMatchMode::CopsAndThieves;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Match", meta=(ClampMin="1",ClampMax="32")) int32 MaxPlayers = 8;
 virtual void PostLogin(APlayerController* NewPlayer) override;
 virtual void Logout(AController* Exiting) override;
 UFUNCTION(BlueprintCallable, Category="Match") bool AssignTeam(APlayerController* Player, ESPTeam RequestedTeam);
 UFUNCTION(BlueprintPure, Category="Match") int32 CountTeam(ESPTeam Team) const;
protected:
 UPROPERTY(BlueprintReadOnly, Category="Match") int32 ConnectedPlayers = 0;
};
