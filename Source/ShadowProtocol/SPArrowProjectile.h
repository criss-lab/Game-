#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPArrowProjectile.generated.h"
class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
UCLASS()
class SHADOWPROTOCOL_API ASPArrowProjectile : public AActor
{
 GENERATED_BODY()
public:
 ASPArrowProjectile();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Arrow") TObjectPtr<USphereComponent> Collision;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Arrow") TObjectPtr<UStaticMeshComponent> ArrowMesh;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Arrow") TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arrow", meta=(ClampMin="0.0")) float Damage = 45.f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arrow", meta=(ClampMin="100.0")) float InitialSpeed = 4200.f;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arrow", meta=(ClampMin="0.0")) float GravityScale = 0.35f;
 UFUNCTION(BlueprintCallable, Category="Arrow") void LaunchInDirection(FVector Direction);
protected:
 virtual void BeginPlay() override;
private:
 UFUNCTION() void OnArrowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 UFUNCTION() void OnArrowHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};