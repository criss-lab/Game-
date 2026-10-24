#include "SPMatchGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameStateBase.h"
ASPMatchGameMode::ASPMatchGameMode() { PlayerStateClass = ASPPlayerState::StaticClass(); bUseSeamlessTravel = true; }
void ASPMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
 Super::PostLogin(NewPlayer);
 ConnectedPlayers++;
 if (MatchMode == ESPMatchMode::CopsAndThieves)
 {
  AssignTeam(NewPlayer, CountTeam(ESPTeam::Thieves) <= CountTeam(ESPTeam::Police) ? ESPTeam::Thieves : ESPTeam::Police);
 }
}
void ASPMatchGameMode::Logout(AController* Exiting) { ConnectedPlayers = FMath::Max(0, ConnectedPlayers-1); Super::Logout(Exiting); }
int32 ASPMatchGameMode::CountTeam(ESPTeam Team) const
{
 int32 Count = 0;
 if (!GameState) return Count;
 for (APlayerState* State : GameState->PlayerArray)
 {
  const ASPPlayerState* SP = Cast<ASPPlayerState>(State);
  if (SP && SP->GetTeam() == Team) ++Count;
 }
 return Count;
}
bool ASPMatchGameMode::AssignTeam(APlayerController* Player, ESPTeam RequestedTeam)
{
 if (!HasAuthority() || !IsValid(Player) || RequestedTeam == ESPTeam::Unassigned) return false;
 ASPPlayerState* State = Player->GetPlayerState<ASPPlayerState>();
 if (!State || MatchMode != ESPMatchMode::CopsAndThieves) return false;
 const int32 Limit = FMath::Max(1, MaxPlayers / 2);
 if (State->GetTeam() != RequestedTeam && CountTeam(RequestedTeam) >= Limit) return false;
 State->SetTeam(RequestedTeam);
 return true;
}
