#pragma once
#include "CoreMinimal.h"
#include "SPCharacter.h"
#include "SPPlayerState.h"
#include "SPCombatCharacter.generated.h"
class USPHealthComponent;
UCLASS()
class SHADOWPROTOCOL_API ASPCombatCharacter : public ASPCharacter
{
 GENERATED_BODY()
public:
 ASPCombatCharacter();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat") TObjectPtr<USPHealthComponent> HealthComponent;
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat") ESPTeam BotTeam = ESPTeam::Unassigned;
 UFUNCTION(BlueprintPure, Category="Combat") ESPTeam GetCombatTeam() const;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0", ClampMax="300.0")) float AttackRange = 180.f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0", ClampMax="100.0")) float AttackDamage = 20.f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.1")) float AttackCooldown = 0.7f;
 virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
 UFUNCTION(BlueprintCallable, Category="Combat") void RequestMeleeAttack();
 UFUNCTION(BlueprintImplementableEvent, Category="Combat") void OnAttackPerformed();
private:
 UFUNCTION(Server, Reliable) void ServerMeleeAttack();
 UFUNCTION(NetMulticast, Unreliable) void MulticastAttackPerformed();
 void ExecuteMeleeAttack();
 double LastAttackTime = -1000.0;
};
