#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPPlayerState.generated.h"
UENUM(BlueprintType)
enum class ESPTeam : uint8 { Unassigned, Thieves, Police };
UCLASS()
class SHADOWPROTOCOL_API ASPPlayerState : public APlayerState
{
 GENERATED_BODY()
public:
 ASPPlayerState();
 UPROPERTY(ReplicatedUsing=OnRep_Team, BlueprintReadOnly, Category="Match") ESPTeam Team = ESPTeam::Unassigned;
 UFUNCTION(BlueprintPure, Category="Match") ESPTeam GetTeam() const { return Team; }
 void SetTeam(ESPTeam NewTeam);
 UFUNCTION(BlueprintImplementableEvent, Category="Match") void OnTeamChanged();
 UFUNCTION() void OnRep_Team();
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
