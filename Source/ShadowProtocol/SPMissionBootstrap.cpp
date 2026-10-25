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
 if (EnvironmentClass || EnvironmentBiome != ESPEnvironmentBiome::City || EnvironmentScatterCount > 0 || EnvironmentGroundMesh || EnvironmentPrimaryMesh || EnvironmentSecondaryMesh)
 {
  const TSubclassOf<ASPEnvironmentZone> ZoneType = EnvironmentClass ? EnvironmentClass : ASPEnvironmentZone::StaticClass();
  ASPEnvironmentZone* Zone = GetWorld()->SpawnActor<ASPEnvironmentZone>(ZoneType, GetActorTransform().TransformPosition(EnvironmentOffset), GetActorRotation());
  if (Zone)
  {
   Zone->Biome = EnvironmentBiome;
   Zone->GroundMesh = EnvironmentGroundMesh;
   Zone->PrimaryMesh = EnvironmentPrimaryMesh;
   Zone->SecondaryMesh = EnvironmentSecondaryMesh;
   Zone->ScatterCount = EnvironmentScatterCount;
   Zone->Radius = EnvironmentRadius;
   Zone->Seed = EnvironmentSeed;
   Zone->BuildEnvironment();
  }
 }
 ASPWorldMachine* MissionConsole = nullptr;
 {
  const TSubclassOf<ASPWorldMachine> MachineType = WorldMachineClass ? WorldMachineClass : ASPWorldMachine::StaticClass();
  const int32 SafeMachines = FMath::Clamp(MachineCount, 0, 12);
  for (int32 Index = 0; Index < SafeMachines; ++Index)
  {
   const FVector Offset = MachineStart + FVector((Index / 3) * 500.f, (Index % 3) * 450.f, 0.f);
   ASPWorldMachine* Machine = GetWorld()->SpawnActor<ASPWorldMachine>(MachineType, GetActorTransform().TransformPosition(Offset), GetActorRotation());
   if (Machine)
   {
    Machine->MachineType = MissionMachineTypes.IsValidIndex(Index) ? MissionMachineTypes[Index] : ESPWorldMachineType::SecurityConsole;
    if (!MissionConsole && Machine->MachineType == ESPWorldMachineType::SecurityConsole) MissionConsole = Machine;
   }
  }
 }
 if (StreetClass)
 {
  ASPStreetBlock* Street = GetWorld()->SpawnActor<ASPStreetBlock>(StreetClass, GetActorLocation()+StreetOffset, GetActorRotation());
  if (Street) Street->RebuildBlock();
 }
 {
  const TSubclassOf<ASPCombatCharacter> BotType = EnemyClass ? EnemyClass : ASPCombatCharacter::StaticClass();
  const int32 SafeCount = FMath::Clamp(EnemyCount, 0, 12);
  for (int32 Index = 0; Index < SafeCount; ++Index)
  {
   const FVector Position = GetActorTransform().TransformPosition(EnemyStart + FVector(Index*180.f, (Index%2)*260.f, 0.f));
   ASPCombatCharacter* Enemy = GetWorld()->SpawnActor<ASPCombatCharacter>(BotType, Position, GetActorRotation());
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
 if (HelicopterClass) GetWorld()->SpawnActor<ASPDriveableHelicopter>(HelicopterClass, GetActorTransform().TransformPosition(HelicopterPosition), GetActorRotation());
 if (Director)
 {
  const TSubclassOf<ASPMissionObjective> StageType = ObjectiveClass ? ObjectiveClass : ASPMissionObjective::StaticClass();
  ASPMissionObjective* Objective = GetWorld()->SpawnActor<ASPMissionObjective>(StageType, GetActorTransform().TransformPosition(FirstObjectivePosition), GetActorRotation());
  if (Objective) { Objective->Director = Director; Objective->AdvanceTo = ESPScene::Infiltration; }
  const ESPScene Stages[] = { ESPScene::Ambush, ESPScene::BossFight, ESPScene::VehicleEscape, ESPScene::Debrief };
  const FVector Positions[] = { MachineStart + FVector(0.f, 0.f, 120.f), EnemyStart + FVector(300.f, 0.f, 120.f), EnemyStart + FVector(850.f, 0.f, 120.f), VehiclePosition + FVector(1800.f, 0.f, 0.f) };
  for (int32 Index = 0; Index < 4; ++Index)
  {
   ASPMissionObjective* Stage = GetWorld()->SpawnActor<ASPMissionObjective>(StageType, Origin.TransformPosition(Positions[Index]), GetActorRotation());
   if (!Stage) continue;
   Stage->Director = Director;
   Stage->AdvanceTo = Stages[Index];
   if (Index == 0) { Stage->Requirement = ESPObjectiveRequirement::ActivateMachine; Stage->RequiredMachine = MissionConsole; }
   if (Index == 1 || Index == 2) Stage->Requirement = ESPObjectiveRequirement::ClearEnemies;
   if (Index == 3) Stage->Requirement = ESPObjectiveRequirement::EscapeInVehicle;
  }
 }
}
