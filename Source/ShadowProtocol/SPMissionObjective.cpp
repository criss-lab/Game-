#include "SPMissionObjective.h"
#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
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
void ASPMissionObjective::OnTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 if (!HasAuthority() || bCompleted || !Director) return;
 ASPCombatCharacter* Player = Cast<ASPCombatCharacter>(OtherActor);
 if (!Player || !Player->IsPlayerControlled() || !Player->HealthComponent || !Player->HealthComponent->IsAlive()) return;
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
