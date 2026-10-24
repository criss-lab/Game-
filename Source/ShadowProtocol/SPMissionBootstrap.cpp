#include "SPMissionBootstrap.h"
#include "SPStreetBlock.h"
#include "SPCombatCharacter.h"
#include "SPCombatAIController.h"
#include "SPDriveableVehicle.h"
#include "SPMissionObjective.h"
#include "SPMissionRewards.h"
#include "SPSceneDirector.h"
#include "SPEnvironmentZone.h"
#include "SPWorldMachine.h"
#include "Engine/World.h"
ASPMissionBootstrap::ASPMissionBootstrap()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = true;
 MissionMachineTypes = { ESPWorldMachineType::SecurityConsole, ESPWorldMachineType::CombatTurret, ESPWorldMachineType::ArrowCache };
}
void ASPMissionBootstrap::BeginPlay()
{
 Super::BeginPlay();
 if (HasAuthority()) SpawnGrayboxMission();
}
void ASPMissionBootstrap::SpawnGrayboxMission()
{
 if (!HasAuthority() || bSpawned || !GetWorld()) return;
 bSpawned = true;
 const FTransform Origin = GetActorTransform();
 Director = GetWorld()->SpawnActor<ASPSceneDirector>(ASPSceneDirector::StaticClass(), Origin);
 if (Director && GetNetMode() == NM_Standalone)
 {
  ASPMissionRewards* Rewards = GetWorld()->SpawnActor<ASPMissionRewards>(ASPMissionRewards::StaticClass(), Origin);
  if (Rewards)
  {
   Rewards->Reward.Mission = 1;
   Rewards->Reward.Coins = 500;
   Rewards->Reward.XP = 350;
   Rewards->BindDirector(Director);
  }
 }
 if (EnvironmentClass)
 {
  ASPEnvironmentZone* Zone = GetWorld()->SpawnActor<ASPEnvironmentZone>(EnvironmentClass, GetActorTransform().TransformPosition(EnvironmentOffset), GetActorRotation());
  if (Zone) { Zone->Biome = EnvironmentBiome; Zone->BuildEnvironment(); }
 }
 if (WorldMachineClass)
 {
  const int32 SafeMachines = FMath::Clamp(MachineCount, 0, 12);
  for (int32 Index = 0; Index < SafeMachines; ++Index)
  {
   const FVector Offset = MachineStart + FVector((Index / 3) * 500.f, (Index % 3) * 450.f, 0.f);
   ASPWorldMachine* Machine = GetWorld()->SpawnActor<ASPWorldMachine>(WorldMachineClass, GetActorTransform().TransformPosition(Offset), GetActorRotation());
   if (Machine) Machine->MachineType = MissionMachineTypes.IsValidIndex(Index) ? MissionMachineTypes[Index] : ESPWorldMachineType::SecurityConsole;
  }
 }
 if (StreetClass)
 {
  ASPStreetBlock* Street = GetWorld()->SpawnActor<ASPStreetBlock>(StreetClass, GetActorLocation()+StreetOffset, GetActorRotation());
  if (Street) Street->RebuildBlock();
 }
 if (EnemyClass)
 {
  const int32 SafeCount = FMath::Clamp(EnemyCount, 0, 12);
  for (int32 Index = 0; Index < SafeCount; ++Index)
  {
   const FVector Position = GetActorTransform().TransformPosition(EnemyStart + FVector(Index*180.f, (Index%2)*260.f, 0.f));
   ASPCombatCharacter* Enemy = GetWorld()->SpawnActor<ASPCombatCharacter>(EnemyClass, Position, GetActorRotation());
   if (Enemy)
   {
    Enemy->BotTeam = ESPTeam::Thieves;
    const TSubclassOf<ASPCombatAIController> ControllerType = EnemyControllerClass ? EnemyControllerClass : ASPCombatAIController::StaticClass();
    ASPCombatAIController* AI = GetWorld()->SpawnActor<ASPCombatAIController>(ControllerType, Position, GetActorRotation());
    if (AI) AI->Possess(Enemy);
   }
  }
 }
 if (VehicleClass) GetWorld()->SpawnActor<ASPDriveableVehicle>(VehicleClass, GetActorTransform().TransformPosition(VehiclePosition), GetActorRotation());
 if (ObjectiveClass && Director)
 {
  ASPMissionObjective* Objective = GetWorld()->SpawnActor<ASPMissionObjective>(ObjectiveClass, GetActorTransform().TransformPosition(FirstObjectivePosition), GetActorRotation());
  if (Objective) { Objective->Director = Director; Objective->AdvanceTo = ESPScene::Infiltration; }
 }
}
