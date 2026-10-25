#include "SPHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "SPCombatCharacter.h"
USPHealthComponent::USPHealthComponent()
{
 SetIsReplicatedByDefault(true);
 PrimaryComponentTick.bCanEverTick = false;
}
void USPHealthComponent::BeginPlay()
{
 Super::BeginPlay();
 if (GetOwner() && GetOwner()->HasAuthority())
 {
  MaxHealth = FMath::Max(1.f, MaxHealth);
  Health = FMath::Clamp(Health, 0.f, MaxHealth);
  GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USPHealthComponent::HandleAnyDamage);
 }
 OnRep_Health();
}
void USPHealthComponent::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
 if (!GetOwner() || !GetOwner()->HasAuthority() || !FMath::IsFinite(Damage) || Damage <= 0.f || !IsAlive()) return;
 float FinalDamage = Damage;
 const ASPCombatCharacter* Defender = Cast<ASPCombatCharacter>(GetOwner());
 if (Defender && Defender->bBlocking && DamageCauser)
 {
  const FVector ToAttacker = (DamageCauser->GetActorLocation() - Defender->GetActorLocation()).GetSafeNormal2D();
  if (FVector::DotProduct(Defender->GetActorForwardVector().GetSafeNormal2D(), ToAttacker) > 0.2f)
   FinalDamage *= 0.3f;
 }
 Health = FMath::Clamp(Health - FinalDamage, 0.f, MaxHealth);
 OnRep_Health();
 GetOwner()->ForceNetUpdate();
}
float USPHealthComponent::RestoreHealth(float Amount)
{
 if (!GetOwner() || !GetOwner()->HasAuthority() || !FMath::IsFinite(Amount) || Amount <= 0.f || !IsAlive()) return 0.f;
 const float Before = Health;
 Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
 OnRep_Health();
 GetOwner()->ForceNetUpdate();
 return Health - Before;
}
void USPHealthComponent::OnRep_Health() { OnHealthChanged.Broadcast(Health, MaxHealth); }
void USPHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(USPHealthComponent, Health);
}
