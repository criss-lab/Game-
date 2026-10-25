#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPGarageSpawner.generated.h"
class ASPDriveableVehicle;
USTRUCT(BlueprintType)
struct FSPGarageVehicle
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CarId = NAME_None;
 UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<ASPDriveableVehicle> VehicleClass;
};
UCLASS()
class SHADOWPROTOCOL_API ASPGarageSpawner : public AActor
{
 GENERATED_BODY()
public:
 ASPGarageSpawner();
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Garage") TArray<FSPGarageVehicle> Vehicles;
 UFUNCTION(BlueprintCallable, Category="Garage") ASPDriveableVehicle* SpawnEquippedCar();
 UFUNCTION(BlueprintImplementableEvent, Category="Garage") void OnCarSpawned(ASPDriveableVehicle* Car);
};
