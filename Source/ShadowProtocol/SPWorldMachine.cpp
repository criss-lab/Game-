#include "SPWorldMachine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
ASPWorldMachine::ASPWorldMachine()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = true;
 SetReplicateMovement(true);
 SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
 SetRootComponent(SceneRoot);
 MachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MachineMesh"));
 MachineMesh->SetupAttachment(SceneRoot);
 MachineMesh->SetCollisionProfileName(TEXT("BlockAll"));
 InteractionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
 InteractionRadius->SetupAttachment(SceneRoot);
 InteractionRadius->SetSphereRadius(220.f);
 InteractionRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
 InteractionRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
 InteractionRadius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
 DisplayName = FText::FromString(TEXT("Field Machine"));
}
bool ASPWorldMachine::CanInteract(AActor* User) const
{
 if (!HasAuthority() || !IsValid(User) || User == this || !InteractionRadius || !GetWorld()) return false;
 if (FVector::DistSquared(User->GetActorLocation(), GetActorLocation()) > FMath::Square(InteractionRadius->GetScaledSphereRadius())) return false;
 if (!bCanToggle && bActivated) return false;
 return GetWorld()->GetTimeSeconds() - LastUseTime >= FMath::Max(0.f, InteractionCooldown);
}
void ASPWorldMachine::Interact(AActor* User)
{
 if (!CanInteract(User)) return;
 LastUseTime = GetWorld()->GetTimeSeconds();
 if (ASPCombatCharacter* Character = Cast<ASPCombatCharacter>(User))
 {
  if (MachineType == ESPWorldMachineType::ArrowCache || MachineType == ESPWorldMachineType::BowRack)
  {
   Character->ArrowAmmo = FMath::Min(12, Character->ArrowAmmo + 6);
   Character->OnArrowAmmoChanged(Character->ArrowAmmo);
   Character->ForceNetUpdate();
  }
  else if (MachineType == ESPWorldMachineType::MedicalStation && Character->HealthComponent)
  {
   Character->HealthComponent->RestoreHealth(35.f);
  }
 }
 bActivated = bCanToggle ? !bActivated : true;
 OnMachineUsed(User, bActivated);
 ForceNetUpdate();
}
void ASPWorldMachine::OnRep_Activated() { }
void ASPWorldMachine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPWorldMachine, bActivated);
}
