#include "SPGarageSpawner.h"
#include "SPDriveableVehicle.h"
#include "SPProgressionSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
ASPGarageSpawner::ASPGarageSpawner() { PrimaryActorTick.bCanEverTick = false; }
ASPDriveableVehicle* ASPGarageSpawner::SpawnEquippedCar()
{
 if (GetNetMode() != NM_Standalone || !GetWorld()) return nullptr;
 UGameInstance* Instance = GetGameInstance();
 USPProgressionSubsystem* Progress = Instance ? Instance->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 if (!Progress) return nullptr;
 const FName Selected = Progress->GetEquippedCar();
 if (!Progress->OwnsCar(Selected)) return nullptr;
 const FSPGarageVehicle* Entry = Vehicles.FindByPredicate([Selected](const FSPGarageVehicle& Item) { return Item.CarId == Selected; });
 if (!Entry || !Entry->VehicleClass) return nullptr;
 ASPDriveableVehicle* Car = GetWorld()->SpawnActor<ASPDriveableVehicle>(Entry->VehicleClass, GetActorTransform());
 if (Car) OnCarSpawned(Car);
 return Car;
}
