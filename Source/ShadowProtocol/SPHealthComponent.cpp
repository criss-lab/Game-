#include "SPHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "SPCombatCharacter.h"
USPHealthComponent::USPHealthComponent() { SetIsReplicatedByDefault(true); PrimaryComponentTick.bCanEverTick = false; }
void USPHealthComponent::BeginPlay()
{
 Super::BeginPlay();
 if (GetOwner() && GetOwner()->HasAuthority())
 {
  Health = FMath::Max(0.f, MaxHealth);
  GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USPHealthComponent::HandleAnyDamage);
 }
 OnRep_Health();
}
void USPHealthComponent::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
 if (!GetOwner() || !GetOwner()->HasAuthority() || !FMath::IsFinite(Damage) || Damage <= 0.f || !IsAlive()) return;
 Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
 OnRep_Health();
 GetOwner()->ForceNetUpdate();
}
void USPHealthComponent::OnRep_Health() { OnHealthChanged.Broadcast(Health, MaxHealth); }
void USPHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(USPHealthComponent, Health);
}
