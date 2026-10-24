#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "SPArrowProjectile.h"
#include "SPWorldMachine.h"
#include "SPDriveableVehicle.h"
#include "SPDriveableHelicopter.h"
#include "Camera/CameraComponent.h"
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
 Input->BindAction(TEXT("Interact"), IE_Pressed, this, &ASPCombatCharacter::InteractWithWorld);
 Input->BindAction(TEXT("ArrowFire"), IE_Pressed, this, &ASPCombatCharacter::FireArrow);
}
void ASPCombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPCombatCharacter, bBlocking);
 DOREPLIFETIME(ASPCombatCharacter, ArrowAmmo);
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
 if (HasAuthority()) ExecuteMeleeAttack(); else ServerMeleeAttack();
}
void ASPCombatCharacter::ServerMeleeAttack_Implementation() { ExecuteMeleeAttack(); }
void ASPCombatCharacter::MulticastAttackPerformed_Implementation() { OnAttackPerformed(); }
void ASPCombatCharacter::InteractWithWorld()
{
 if (!IsLocallyControlled() && !HasAuthority()) return;
 if (HasAuthority()) ExecuteInteraction(); else ServerInteractWithWorld();
}
void ASPCombatCharacter::ServerInteractWithWorld_Implementation() { ExecuteInteraction(); }
void ASPCombatCharacter::ExecuteInteraction()
{
 if (!HasAuthority() || !GetWorld()) return;
 FVector ViewLocation = GetActorLocation() + FVector(0.f, 0.f, 60.f);
 FRotator ViewRotation = GetActorRotation();
 if (Controller) Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
 const FVector End = ViewLocation + ViewRotation.Vector() * 500.f;
 FHitResult Hit;
 FCollisionQueryParams Params(SCENE_QUERY_STAT(SPWorldInteraction), true, this);
 if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, End, ECC_Visibility, Params))
 {
  if (ASPDriveableHelicopter* Helicopter = Cast<ASPDriveableHelicopter>(Hit.GetActor()))
  {
   Helicopter->EnterHelicopter(this);
   return;
  }
  if (ASPDriveableVehicle* Vehicle = Cast<ASPDriveableVehicle>(Hit.GetActor()))
  {
   Vehicle->EnterVehicle(this);
   return;
  }
  if (ASPWorldMachine* Machine = Cast<ASPWorldMachine>(Hit.GetActor()))
  {
   Machine->Interact(this);
   return;
  }
 }
 // Fallback lets players activate a nearby machine even when its mesh has no collision.
 TArray<FOverlapResult> Nearby;
 FCollisionQueryParams OverlapParams(SCENE_QUERY_STAT(SPNearbyMachine), false, this);
 GetWorld()->OverlapMultiByObjectType(Nearby, GetActorLocation(), FQuat::Identity,
  FCollisionObjectQueryParams(ECC_WorldDynamic), FCollisionShape::MakeSphere(220.f), OverlapParams);
 ASPWorldMachine* Best = nullptr;
 float BestDistance = TNumericLimits<float>::Max();
 for (const FOverlapResult& Result : Nearby)
 {
  ASPWorldMachine* Machine = Cast<ASPWorldMachine>(Result.GetActor());
  if (!Machine || !Machine->CanInteract(this)) continue;
  const float Distance = FVector::DistSquared(GetActorLocation(), Machine->GetActorLocation());
  if (Distance < BestDistance) { BestDistance = Distance; Best = Machine; }
 }
 if (Best) Best->Interact(this);
}
void ASPCombatCharacter::FireArrow()
{
 if (!IsLocallyControlled() && !HasAuthority()) return;
 if (HasAuthority()) ExecuteArrowShot(); else ServerFireArrow();
}
void ASPCombatCharacter::ServerFireArrow_Implementation() { ExecuteArrowShot(); }
void ASPCombatCharacter::ExecuteArrowShot()
{
 if (!HasAuthority() || !GetWorld() || !ArrowClass || ArrowAmmo <= 0 || !HealthComponent || !HealthComponent->IsAlive()) return;
 const double Now = GetWorld()->GetTimeSeconds();
 if (Now - LastArrowTime < FMath::Max(0.1f, ArrowCooldown)) return;
 LastArrowTime = Now;
 FVector ViewLocation = GetActorLocation() + FVector(0.f, 0.f, 60.f);
 FRotator ViewRotation = GetActorRotation();
 if (Controller) Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
 const FVector Direction = ViewRotation.Vector().GetSafeNormal();
 const FVector SpawnLocation = ViewLocation + Direction * 75.f;
 FActorSpawnParameters SpawnParams;
 SpawnParams.Owner = this;
 SpawnParams.Instigator = this;
 SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
 ASPArrowProjectile* Arrow = GetWorld()->SpawnActor<ASPArrowProjectile>(ArrowClass, SpawnLocation, Direction.Rotation(), SpawnParams);
 if (!Arrow) return;
 ArrowAmmo = FMath::Max(0, ArrowAmmo - 1);
 OnArrowAmmoChanged(ArrowAmmo);
 Arrow->LaunchInDirection(Direction);
 ForceNetUpdate();
}
void ASPCombatCharacter::OnRep_ArrowAmmo() { OnArrowAmmoChanged(ArrowAmmo); }
void ASPCombatCharacter::ExecuteMeleeAttack()
{
 if (!HasAuthority() || !HealthComponent || !HealthComponent->IsAlive() || !GetWorld()) return;
 const double Now = GetWorld()->GetTimeSeconds();
 if (Now - LastAttackTime < AttackCooldown) return;
 LastAttackTime = Now;
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
 if (BestVictim) UGameplayStatics::ApplyDamage(BestVictim, FMath::Max(0.f, AttackDamage), GetController(), this, UDamageType::StaticClass());
 MulticastAttackPerformed();
}
