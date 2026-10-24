#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
ASPCombatCharacter::ASPCombatCharacter()
{
 bReplicates = true;
 HealthComponent = CreateDefaultSubobject<USPHealthComponent>(TEXT("HealthComponent"));
}
ESPTeam ASPCombatCharacter::GetCombatTeam() const
{
 const ASPPlayerState* State = GetPlayerState<ASPPlayerState>();
 return State && State->GetTeam() != ESPTeam::Unassigned ? State->GetTeam() : BotTeam;
}
void ASPCombatCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
 Super::SetupPlayerInputComponent(Input);
 Input->BindAction(TEXT("Melee"), IE_Pressed, this, &ASPCombatCharacter::RequestMeleeAttack);
 Input->BindAction(TEXT("Block"), IE_Pressed, this, &ASPCombatCharacter::StartBlock);
 Input->BindAction(TEXT("Block"), IE_Released, this, &ASPCombatCharacter::StopBlock);
}
void ASPCombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPCombatCharacter, bBlocking);
}
void ASPCombatCharacter::StartBlock() { if (HasAuthority()) bBlocking = true; else ServerSetBlocking(true); }
void ASPCombatCharacter::StopBlock() { if (HasAuthority()) bBlocking = false; else ServerSetBlocking(false); }
void ASPCombatCharacter::ServerSetBlocking_Implementation(bool bNewBlocking)
{
 if (!HealthComponent || !HealthComponent->IsAlive()) { bBlocking = false; return; }
 bBlocking = bNewBlocking;
}
void ASPCombatCharacter::RequestMeleeAttack()
{
 if (!IsLocallyControlled() && !HasAuthority()) return;
 if (HasAuthority()) ExecuteMeleeAttack();
 else ServerMeleeAttack();
}
void ASPCombatCharacter::ServerMeleeAttack_Implementation() { ExecuteMeleeAttack(); }
void ASPCombatCharacter::MulticastAttackPerformed_Implementation() { OnAttackPerformed(); }
void ASPCombatCharacter::ExecuteMeleeAttack()
{
 if (!HasAuthority() || !HealthComponent || !HealthComponent->IsAlive() || !GetWorld()) return;
 const double Now = GetWorld()->GetTimeSeconds();
 if (Now - LastAttackTime < AttackCooldown) return;
 LastAttackTime = Now;
 // Face the nearest valid hostile in a short forward arc before the authoritative hit check.
 const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 55.f);
 const FVector Forward = GetActorForwardVector().GetSafeNormal();
 const FVector End = Start + Forward * FMath::Clamp(AttackRange, 0.f, 300.f);
 TArray<FHitResult> Hits;
 FCollisionQueryParams Params(SCENE_QUERY_STAT(SPMelee), false, this);
 GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn,
  FCollisionShape::MakeSphere(FMath::Clamp(AttackRadius, 1.f, 80.f)), Params);
 ASPCombatCharacter* BestVictim = nullptr;
 float BestDistance = TNumericLimits<float>::Max();
 for (const FHitResult& Hit : Hits)
 {
  ASPCombatCharacter* Victim = Cast<ASPCombatCharacter>(Hit.GetActor());
  if (!Victim || Victim == this || !Victim->HealthComponent || !Victim->HealthComponent->IsAlive()) continue;
  const ESPTeam Mine = GetCombatTeam();
  const ESPTeam Theirs = Victim->GetCombatTeam();
  if (Mine != ESPTeam::Unassigned && Mine == Theirs) continue;
  const FVector ToVictim = Victim->GetActorLocation() - GetActorLocation();
  const float Distance = ToVictim.Size();
  if (Distance > AttackRange + 100.f || FVector::DotProduct(Forward, ToVictim.GetSafeNormal()) < AttackConeDot) continue;
  FHitResult Obstruction;
  FCollisionQueryParams LOSParams(SCENE_QUERY_STAT(SPMeleeLOS), true, this);
  LOSParams.AddIgnoredActor(Victim);
  const bool bBlocked = GetWorld()->LineTraceSingleByChannel(Obstruction, Start, Victim->GetActorLocation() + FVector(0.f,0.f,45.f), ECC_Visibility, LOSParams);
  if (bBlocked) continue;
  if (Distance < BestDistance) { BestDistance = Distance; BestVictim = Victim; }
 }
 if (BestVictim)
 {
  UGameplayStatics::ApplyDamage(BestVictim, FMath::Max(0.f, AttackDamage), GetController(), this, UDamageType::StaticClass());
 }
 MulticastAttackPerformed();
}
