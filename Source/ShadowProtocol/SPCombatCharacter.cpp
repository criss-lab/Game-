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
void ASPCombatCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
 Super::SetupPlayerInputComponent(Input);
 Input->BindAction(TEXT("Melee"), IE_Pressed, this, &ASPCombatCharacter::RequestMeleeAttack);
}
void ASPCombatCharacter::RequestMeleeAttack()
{
 if (HasAuthority()) ExecuteMeleeAttack();
 else ServerMeleeAttack();
}
void ASPCombatCharacter::ServerMeleeAttack_Implementation() { ExecuteMeleeAttack(); }
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
  AActor* Victim = Hit.GetActor();
  if (Victim && Victim != this)
  {
   const ASPCombatCharacter* Other = Cast<ASPCombatCharacter>(Victim);
   const USPHealthComponent* OtherHealth = Other ? Other->HealthComponent.Get() : nullptr;
   if (!OtherHealth || !OtherHealth->IsAlive()) return;
   UGameplayStatics::ApplyDamage(Victim, AttackDamage, GetController(), this, UDamageType::StaticClass());
  }
 }
 OnAttackPerformed();
}
