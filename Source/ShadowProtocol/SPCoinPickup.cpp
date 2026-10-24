#include "SPCoinPickup.h"
#include "SPCombatCharacter.h"
#include "SPProgressionSubsystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/GameInstance.h"
ASPCoinPickup::ASPCoinPickup()
{
 PrimaryActorTick.bCanEverTick = false;
 PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
 SetRootComponent(PickupTrigger);
 PickupTrigger->SetSphereRadius(85.f);
 PickupTrigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
 PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASPCoinPickup::HandleOverlap);
}
void ASPCoinPickup::HandleOverlap(UPrimitiveComponent* Component, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
 // Offline-only reward path. Online rewards must be issued by an authoritative backend.
 ASPCombatCharacter* Character = Cast<ASPCombatCharacter>(OtherActor);
 if (!Character || !Character->IsLocallyControlled() || Character->GetNetMode() != NM_Standalone) return;
 UGameInstance* Instance = GetGameInstance();
 USPProgressionSubsystem* Progress = Instance ? Instance->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 if (!Progress || !Progress->CollectPickup(PickupId.IsNone() ? FName(*GetPathName()) : PickupId, CoinValue)) return;
 PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 Collected();
 Destroy();
}
