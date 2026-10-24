#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPSceneDirector.h"
#include "SPMissionObjective.generated.h"
class UBoxComponent;
UCLASS()
class SHADOWPROTOCOL_API ASPMissionObjective : public AActor
{
 GENERATED_BODY()
public:
 ASPMissionObjective();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mission") TObjectPtr<UBoxComponent> Trigger;
 UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Mission") TObjectPtr<ASPSceneDirector> Director;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission") ESPScene AdvanceTo = ESPScene::Infiltration;
 UPROPERTY(ReplicatedUsing=OnRep_Completed, BlueprintReadOnly, Category="Mission") bool bCompleted = false;
 UFUNCTION(BlueprintImplementableEvent, Category="Mission") void ObjectiveCompleted();
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
 UFUNCTION() void OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
 UFUNCTION() void OnRep_Completed();
};
