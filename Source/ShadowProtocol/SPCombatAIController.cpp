#include "SPCombatAIController.h"
#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"
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
 APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
 ASPCombatCharacter* Target = Cast<ASPCombatCharacter>(Player);
 if (!Target || !Target->HealthComponent || !Target->HealthComponent->IsAlive()) { StopMovement(); return; }
 const float Distance = FVector::Dist(Self->GetActorLocation(), Target->GetActorLocation());
 if (Distance > DetectionRadius) { StopMovement(); return; }
 if (Distance > EngagementDistance) { MoveToActor(Target, EngagementDistance * 0.8f); return; }
 StopMovement();
 const FVector Direction = Target->GetActorLocation() - Self->GetActorLocation();
 Self->SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
 Self->RequestMeleeAttack();
}
