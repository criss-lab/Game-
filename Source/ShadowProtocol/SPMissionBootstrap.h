#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPEnvironmentZone.h"
#include "SPWorldMachine.h"
#include "SPMissionBootstrap.generated.h"
class ASPStreetBlock;
class ASPCombatCharacter;
class ASPCombatAIController;
class ASPSceneDirector;
class ASPMissionObjective;
class ASPDriveableVehicle;
UCLASS()
class SHADOWPROTOCOL_API ASPMissionBootstrap : public AActor
{
 GENERATED_BODY()
public:
 ASPMissionBootstrap();
 UPROPERTY(EditAnywhere, Category="Mission") TSubclassOf<ASPStreetBlock> StreetClass;
 UPROPERTY(EditAnywhere, Category="Mission") TSubclassOf<ASPCombatCharacter> EnemyClass;
 UPROPERTY(EditAnywhere, Category="Mission") TSubclassOf<ASPCombatAIController> EnemyControllerClass;
 UPROPERTY(EditAnywhere, Category="Mission") TSubclassOf<ASPDriveableVehicle> VehicleClass;
 UPROPERTY(EditAnywhere, Category="Mission") TSubclassOf<ASPMissionObjective> ObjectiveClass;
 UPROPERTY(EditAnywhere, Category="Environment") TSubclassOf<ASPEnvironmentZone> EnvironmentClass;
 UPROPERTY(EditAnywhere, Category="Environment") ESPEnvironmentBiome EnvironmentBiome = ESPEnvironmentBiome::City;
 UPROPERTY(EditAnywhere, Category="Environment") FVector EnvironmentOffset = FVector(0.f, 0.f, 0.f);
 UPROPERTY(EditAnywhere, Category="Environment", meta=(ClampMin="0", ClampMax="12")) int32 MachineCount = 3;
 UPROPERTY(EditAnywhere, Category="Environment") TSubclassOf<ASPWorldMachine> WorldMachineClass;
 UPROPERTY(EditAnywhere, Category="Environment") TArray<ESPWorldMachineType> MissionMachineTypes = { ESPWorldMachineType::SecurityConsole, ESPWorldMachineType::CombatTurret, ESPWorldMachineType::ArrowCache };
 UPROPERTY(EditAnywhere, Category="Environment") FVector MachineStart = FVector(850.f, 600.f, 50.f);
 UPROPERTY(EditAnywhere, Category="Mission") int32 EnemyCount = 3;
 UPROPERTY(EditAnywhere, Category="Mission") FVector StreetOffset = FVector::ZeroVector;
 UPROPERTY(EditAnywhere, Category="Mission") FVector EnemyStart = FVector(1200.f, 300.f, 120.f);
 UPROPERTY(EditAnywhere, Category="Mission") FVector VehiclePosition = FVector(3500.f, 0.f, 130.f);
 UPROPERTY(EditAnywhere, Category="Mission") FVector FirstObjectivePosition = FVector(650.f, 0.f, 120.f);
 UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Mission") void SpawnGrayboxMission();
protected:
 virtual void BeginPlay() override;
private:
 UPROPERTY() TObjectPtr<ASPSceneDirector> Director;
 bool bSpawned = false;
};
