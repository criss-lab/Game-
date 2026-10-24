#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPWorldMachine.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class ESPWorldMachineType : uint8
{
 SecurityConsole,
 CombatTurret,
 Vehicle,
 Helicopter,
 Trailer,
 BowRack,
 ArrowCache,
 MedicalStation,
 Generator,
 GateControl
};

UCLASS()
class SHADOWPROTOCOL_API ASPWorldMachine : public AActor
{
 GENERATED_BODY()
public:
 ASPWorldMachine();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Machine") TObjectPtr<USceneComponent> SceneRoot;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Machine") TObjectPtr<UStaticMeshComponent> MachineMesh;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Machine") TObjectPtr<USphereComponent> InteractionRadius;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Machine") ESPWorldMachineType MachineType = ESPWorldMachineType::SecurityConsole;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Machine") FText DisplayName;
 UPROPERTY(ReplicatedUsing=OnRep_Activated, BlueprintReadOnly, Category="Machine") bool bActivated = false;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Machine") bool bCanToggle = true;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Machine", meta=(ClampMin="0.0")) float InteractionCooldown = 1.0f;
 UFUNCTION(BlueprintCallable, Category="Machine") bool CanInteract(AActor* User) const;
 UFUNCTION(BlueprintCallable, Category="Machine") void Interact(AActor* User);
 UFUNCTION(BlueprintImplementableEvent, Category="Machine") void OnMachineUsed(AActor* User, bool bNowActivated);
 UFUNCTION() void OnRep_Activated();
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
 double LastUseTime = -1000.0;
};
