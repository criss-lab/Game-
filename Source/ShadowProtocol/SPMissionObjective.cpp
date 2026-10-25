#include "SPMissionObjective.h"
#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "SPWorldMachine.h"
#include "SPDriveableVehicle.h"
#include "SPDriveableHelicopter.h"
#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
ASPMissionObjective::ASPMissionObjective()
{
 bReplicates = true;
 PrimaryActorTick.bCanEverTick = false;
 Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ObjectiveTrigger"));
 RootComponent = Trigger;
 Trigger->SetBoxExtent(FVector(120.f,120.f,120.f));
 Trigger->SetCollisionProfileName(TEXT("Trigger"));
 Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASPMissionObjective::OnTrigger);
}
void ASPMissionObjective::BeginPlay()
{
 Super::BeginPlay();
 if (HasAuthority())
 {
  FTimerHandle CheckTimer;
  GetWorldTimerManager().SetTimer(CheckTimer, this, &ASPMissionObjective::CheckOccupants, 0.25f, true);
 }
}
void ASPMissionObjective::CheckOccupants()
{
 if (bCompleted || !Director || static_cast<uint8>(AdvanceTo) != static_cast<uint8>(Director->CurrentScene) + 1) return;
 TArray<AActor*> Occupants;
 Trigger->GetOverlappingActors(Occupants, APawn::StaticClass());
 for (AActor* Occupant : Occupants)
 {
  OnTrigger(Trigger, Occupant, nullptr, INDEX_NONE, false, FHitResult());
  if (bCompleted) break;
 }
}
void ASPMissionObjective::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 if (!HasAuthority() || bCompleted || !Director) return;
 ASPCombatCharacter* Player = Cast<ASPCombatCharacter>(OtherActor);
 if (Requirement == ESPObjectiveRequirement::EscapeInVehicle)
 {
  APawn* Vehicle = Cast<ASPDriveableVehicle>(OtherActor);
  if (!Vehicle) Vehicle = Cast<ASPDriveableHelicopter>(OtherActor);
  if (!Vehicle || !Vehicle->IsPlayerControlled()) return;
 }
 else if (!Player || !Player->IsPlayerControlled() || !Player->HealthComponent || !Player->HealthComponent->IsAlive()) return;
 if (Requirement == ESPObjectiveRequirement::ActivateMachine && (!IsValid(RequiredMachine) || !RequiredMachine->bActivated)) return;
 if (Requirement == ESPObjectiveRequirement::ClearEnemies)
 {
  for (TActorIterator<ASPCombatCharacter> It(GetWorld()); It; ++It)
  {
   ASPCombatCharacter* Enemy = *It;
   if (Enemy && !Enemy->IsPlayerControlled() && Enemy->HealthComponent && Enemy->HealthComponent->IsAlive()
       && FVector::DistSquared(Enemy->GetActorLocation(), GetActorLocation()) <= FMath::Square(FMath::Max(100.f, EnemyClearRadius))) return;
  }
 }
 if (static_cast<uint8>(AdvanceTo) != static_cast<uint8>(Director->CurrentScene) + 1) return;
 Director->AdvanceScene(AdvanceTo);
 bCompleted = true;
 OnRep_Completed();
 ForceNetUpdate();
}
void ASPMissionObjective::OnRep_Completed() { if (bCompleted) { Trigger->SetGenerateOverlapEvents(false); ObjectiveCompleted(); } }
void ASPMissionObjective::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPMissionObjective, bCompleted);
}
