#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPCoinPickup.generated.h"
class USphereComponent;
UCLASS()
class SHADOWPROTOCOL_API ASPCoinPickup : public AActor
{
 GENERATED_BODY()
public:
 ASPCoinPickup();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USphereComponent> PickupTrigger;
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reward", meta=(ClampMin="1", ClampMax="1000")) int32 CoinValue = 10;
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Reward") FName PickupId = NAME_None;
 UFUNCTION(BlueprintImplementableEvent, Category="Reward") void Collected();
private:
 UFUNCTION() void HandleOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);
};
