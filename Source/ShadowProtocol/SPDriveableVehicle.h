#pragma once
#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SPDriveableVehicle.generated.h"
UCLASS()
class SHADOWPROTOCOL_API ASPDriveableVehicle : public AWheeledVehiclePawn
{
 GENERATED_BODY()
public:
 ASPDriveableVehicle();
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vehicle") FText VehicleDisplayName;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vehicle") float MaxVisualDamage = 100.f;
 UPROPERTY(BlueprintReadOnly, Category="Vehicle") TObjectPtr<APawn> PreviousDriverPawn;
 UPROPERTY(BlueprintReadOnly, Category="Vehicle") TObjectPtr<AController> PreviousDriverController;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle") FVector ExitOffset = FVector(0.f, 220.f, 80.f);
 UFUNCTION(BlueprintImplementableEvent, Category="Vehicle") void OnVehicleOccupied(APawn* PreviousPawn);
 UFUNCTION(BlueprintImplementableEvent, Category="Vehicle") void OnVehicleVacated(APawn* PreviousPawn);
 UFUNCTION(BlueprintCallable, Category="Vehicle") bool EnterVehicle(APawn* Passenger);
 UFUNCTION(BlueprintCallable, Category="Vehicle") void ExitVehicle();
 virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
 UFUNCTION(Server, Reliable) void ServerExitVehicle();
 void ExitVehicleAuthority();
 UFUNCTION(BlueprintCallable, Category="Vehicle") bool CanEnterVehicle(APawn* Passenger) const;
};
