#include "SPWorldMachine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "SPCombatCharacter.h"
#include "SPHealthComponent.h"
#include "SPDriveableVehicle.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
ASPWorldMachine::ASPWorldMachine()
{
 PrimaryActorTick.bCanEverTick = true;
 PrimaryActorTick.bStartWithTickEnabled = false;
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
 if (MachineType == ESPWorldMachineType::Trailer)
 {
  TArray<AActor*> Vehicles;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASPDriveableVehicle::StaticClass(), Vehicles);
  ASPDriveableVehicle* Nearest = nullptr;
  float BestDistance = FMath::Square(700.f);
  for (AActor* Candidate : Vehicles)
  {
   const float Distance = FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation());
   if (Distance < BestDistance) { BestDistance = Distance; Nearest = Cast<ASPDriveableVehicle>(Candidate); }
  }
  if (!bActivated && !Nearest) return;
  if (bActivated) DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
  else { AttachToActor(Nearest, FAttachmentTransformRules::KeepWorldTransform); SetActorRelativeLocation(FVector(-350.f,0.f,0.f)); }
 }
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
 SetActorTickEnabled(MachineType == ESPWorldMachineType::CombatTurret && bActivated);
 OnMachineUsed(User, bActivated);
 OnMachineStateChanged(bActivated);
 ForceNetUpdate();
}
void ASPWorldMachine::OnRep_Activated() { OnMachineStateChanged(bActivated); }
void ASPWorldMachine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPWorldMachine, bActivated);
}

void ASPWorldMachine::Tick(float DeltaSeconds)
{
 Super::Tick(DeltaSeconds);
 if (!HasAuthority() || MachineType != ESPWorldMachineType::CombatTurret || !bActivated || !GetWorld()) return;
 ASPCombatCharacter* BestTarget = nullptr;
 float BestDistance = FMath::Square(FMath::Max(100.f, TurretRange));
 for (TActorIterator<ASPCombatCharacter> It(GetWorld()); It; ++It)
 {
  ASPCombatCharacter* Candidate = *It;
  if (!IsValid(Candidate) || !Candidate->HealthComponent || !Candidate->HealthComponent->IsAlive()) continue;
  const ESPTeam Team = Candidate->GetCombatTeam();
  if (TurretTeam != ESPTeam::Unassigned && Team == TurretTeam) continue;
  const float Distance = FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation());
  if (Distance < BestDistance) { BestDistance = Distance; BestTarget = Candidate; }
 }
 if (!BestTarget) return;
 const FVector Muzzle = GetActorLocation() + FVector(0.f, 0.f, 80.f);
 const FVector TargetPoint = BestTarget->GetActorLocation() + FVector(0.f, 0.f, 45.f);
 if (MachineMesh) MachineMesh->SetWorldRotation((TargetPoint - Muzzle).Rotation());
 FHitResult Hit;
 FCollisionQueryParams Params(SCENE_QUERY_STAT(SPTurretLOS), true, this);
 const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Muzzle, TargetPoint, ECC_Visibility, Params);
 if (bHit && Hit.GetActor() != BestTarget) return;
 const double Now = GetWorld()->GetTimeSeconds();
 if (Now - LastTurretShotTime < FMath::Max(0.1f, TurretFireInterval)) return;
 LastTurretShotTime = Now;
 UGameplayStatics::ApplyDamage(BestTarget, FMath::Max(0.f, TurretDamage), nullptr, this, UDamageType::StaticClass());
}
