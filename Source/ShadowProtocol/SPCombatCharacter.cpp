#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
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
 const FVector Start = GetActorLocation() + FVector(0,0,55);
 const FVector End = Start + GetActorForwardVector() * AttackRange;
 FHitResult Hit;
 FCollisionQueryParams Params(SCENE_QUERY_STAT(SPMelee), false, this);
 if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(55.f), Params))
 {
  ASPCombatCharacter* Victim = Cast<ASPCombatCharacter>(Hit.GetActor());
  if (Victim && Victim != this && Victim->HealthComponent && Victim->HealthComponent->IsAlive())
  {
   const ESPTeam Mine = GetCombatTeam();
   const ESPTeam Theirs = Victim->GetCombatTeam();
   const bool bSameAssignedTeam = Mine != ESPTeam::Unassigned && Mine == Theirs;
   if (!bSameAssignedTeam)
   {
    UGameplayStatics::ApplyDamage(Victim, AttackDamage, GetController(), this, UDamageType::StaticClass());
   }
  }
 }
 MulticastAttackPerformed();
}
