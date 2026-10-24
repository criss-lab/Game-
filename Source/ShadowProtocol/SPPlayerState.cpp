#include "SPPlayerState.h"
#include "Net/UnrealNetwork.h"
ASPPlayerState::ASPPlayerState() { bReplicates = true; }
void ASPPlayerState::SetTeam(ESPTeam NewTeam) { if (!HasAuthority()) return; Team = NewTeam; OnRep_Team(); ForceNetUpdate(); }
void ASPPlayerState::OnRep_Team() { OnTeamChanged(); }
void ASPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPPlayerState, Team);
}
