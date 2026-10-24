#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPHealthComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSPHealthChanged, float, Health, float, MaxHealth);
UCLASS(ClassGroup=(ShadowProtocol), meta=(BlueprintSpawnableComponent))
class SHADOWPROTOCOL_API USPHealthComponent : public UActorComponent
{
 GENERATED_BODY()
public:
 USPHealthComponent();
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="1.0")) float MaxHealth = 100.f;
 UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Combat") float Health = 100.f;
 UPROPERTY(BlueprintAssignable, Category="Combat") FSPHealthChanged OnHealthChanged;
 UFUNCTION(BlueprintPure, Category="Combat") bool IsAlive() const { return Health > 0.f; }
 virtual void BeginPlay() override;
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
 UFUNCTION() void HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
 UFUNCTION() void OnRep_Health();
};
