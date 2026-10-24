#include "SPCombatAIController.h"
#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
ASPCombatAIController::ASPCombatAIController() { PrimaryActorTick.bCanEverTick = false; }
void ASPCombatAIController::OnPossess(APawn* InPawn)
{
 Super::OnPossess(InPawn);
 if (HasAuthority()) GetWorldTimerManager().SetTimer(ThinkTimer, this, &ASPCombatAIController::Think, 0.25f, true);
}
void ASPCombatAIController::Think()
{
 ASPCombatCharacter* Self = Cast<ASPCombatCharacter>(GetPawn());
 if (!Self || !Self->HealthComponent || !Self->HealthComponent->IsAlive()) { StopMovement(); return; }
 ASPCombatCharacter* Target = nullptr;
 float BestDistance = DetectionRadius;
 for (TActorIterator<ASPCombatCharacter> It(GetWorld()); It; ++It)
 {
  ASPCombatCharacter* Candidate = *It;
  if (Candidate == Self || !Candidate->HealthComponent || !Candidate->HealthComponent->IsAlive()) continue;
  const ESPTeam Mine = Self->GetCombatTeam();
  const ESPTeam Theirs = Candidate->GetCombatTeam();
  if (Mine != ESPTeam::Unassigned && Mine == Theirs) continue;
  // Unassigned bots are not targets in solo mode; player pawns remain eligible.
  if (!Candidate->IsPlayerControlled() && Theirs == ESPTeam::Unassigned) continue;
  const float Distance = FVector::Dist(Self->GetActorLocation(), Candidate->GetActorLocation());
  if (Distance < BestDistance) { Target = Candidate; BestDistance = Distance; }
 }
 if (!Target) { StopMovement(); return; }
 if (BestDistance > EngagementDistance) { MoveToActor(Target, EngagementDistance * 0.8f); return; }
 StopMovement();
 const FVector Direction = Target->GetActorLocation() - Self->GetActorLocation();
 Self->SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
 Self->RequestMeleeAttack();
}
