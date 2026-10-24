#include "SPMissionBootstrap.h"
#include "SPStreetBlock.h"
#include "SPCombatCharacter.h"
#include "SPCombatAIController.h"
#include "SPDriveableVehicle.h"
#include "SPMissionObjective.h"
#include "SPSceneDirector.h"
#include "Engine/World.h"
ASPMissionBootstrap::ASPMissionBootstrap()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = true;
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
    const TSubclassOf<ASPCombatAIController> ControllerType = EnemyControllerClass ? EnemyControllerClass : ASPCombatAIController::StaticClass();
    ASPCombatAIController* AI = GetWorld()->SpawnActor<ASPCombatAIController>(ControllerType, Position, GetActorRotation());
    if (AI) AI->Possess(Enemy);
   }
  }
 }
 if (VehicleClass)
 {
  GetWorld()->SpawnActor<ASPDriveableVehicle>(VehicleClass, GetActorTransform().TransformPosition(VehiclePosition), GetActorRotation());
 }
 if (ObjectiveClass && Director)
 {
  ASPMissionObjective* Objective = GetWorld()->SpawnActor<ASPMissionObjective>(ObjectiveClass, GetActorTransform().TransformPosition(FirstObjectivePosition), GetActorRotation());
  if (Objective)
  {
   Objective->Director = Director;
   Objective->AdvanceTo = ESPScene::Infiltration;
  }
 }
}
