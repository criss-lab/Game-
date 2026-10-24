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
 UFUNCTION(BlueprintImplementableEvent, Category="Vehicle") void OnVehicleOccupied(APawn* PreviousPawn);
 UFUNCTION(BlueprintCallable, Category="Vehicle") bool CanEnterVehicle(APawn* Passenger) const;
};
