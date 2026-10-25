#pragma once
#include "CoreMinimal.h"
#include "SPCharacter.h"
#include "SPPlayerState.h"
#include "SPCombatCharacter.generated.h"
class USPHealthComponent;
class ASPArrowProjectile;
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
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.1")) float AttackCooldown = 0.55f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0", ClampMax="1.0")) float AttackConeDot = 0.25f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.0")) float AttackRadius = 48.f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat") TSubclassOf<ASPArrowProjectile> ArrowClass;
 UPROPERTY(ReplicatedUsing=OnRep_ArrowAmmo, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0")) int32 ArrowAmmo = 12;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat", meta=(ClampMin="0.1")) float ArrowCooldown = 0.7f;
 UFUNCTION(BlueprintImplementableEvent, Category="Combat") void OnArrowAmmoChanged(int32 NewAmmo);
 virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
 UPROPERTY(Replicated, BlueprintReadOnly, Category="Combat") bool bBlocking = false;
 UFUNCTION(BlueprintCallable, Category="Combat") void StartBlock();
 UFUNCTION(BlueprintCallable, Category="Combat") void StopBlock();
 UFUNCTION(BlueprintCallable, Category="Combat") void RequestMeleeAttack();
 UFUNCTION(BlueprintCallable, Category="Combat") void InteractWithWorld();
 UFUNCTION(BlueprintCallable, Category="Combat") void FireArrow();
 UFUNCTION(BlueprintImplementableEvent, Category="Combat") void OnAttackPerformed();
private:
 UFUNCTION(Server, Reliable) void ServerMeleeAttack();
 UFUNCTION(Server, Reliable) void ServerSetBlocking(bool bNewBlocking);
 UFUNCTION(Server, Reliable) void ServerInteractWithWorld();
 UFUNCTION(Server, Reliable) void ServerFireArrow();
 UFUNCTION(NetMulticast, Unreliable) void MulticastAttackPerformed();
 UFUNCTION() void OnRep_ArrowAmmo();
 void ExecuteMeleeAttack();
 void ExecuteInteraction();
 void ExecuteArrowShot();
 double LastAttackTime = -1000.0;
 double LastArrowTime = -1000.0;
};