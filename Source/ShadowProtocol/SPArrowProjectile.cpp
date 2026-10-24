#include "SPArrowProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SPCombatCharacter.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
ASPArrowProjectile::ASPArrowProjectile()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = true;
 SetReplicateMovement(true);
 Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
 SetRootComponent(Collision);
 Collision->InitSphereRadius(5.f);
 Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
 Collision->SetCollisionObjectType(ECC_WorldDynamic);
 Collision->SetCollisionResponseToAllChannels(ECR_Block);
 Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
 ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
 ArrowMesh->SetupAttachment(Collision);
 static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
 if (ArrowAsset.Succeeded()) ArrowMesh->SetStaticMesh(ArrowAsset.Object);
 ArrowMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
 ArrowMesh->SetRelativeScale3D(FVector(0.04f, 0.04f, 0.7f));
 ArrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
 ProjectileMovement->UpdatedComponent = Collision;
 ProjectileMovement->InitialSpeed = InitialSpeed;
 ProjectileMovement->MaxSpeed = InitialSpeed;
 ProjectileMovement->ProjectileGravityScale = GravityScale;
 ProjectileMovement->bRotationFollowsVelocity = true;
 ProjectileMovement->bShouldBounce = false;
 InitialLifeSpan = 8.f;
}
void ASPArrowProjectile::BeginPlay()
{
 Super::BeginPlay();
 Collision->OnComponentBeginOverlap.AddDynamic(this, &ASPArrowProjectile::OnArrowOverlap);
 Collision->OnComponentHit.AddDynamic(this, &ASPArrowProjectile::OnArrowHit);
}
void ASPArrowProjectile::LaunchInDirection(FVector Direction)
{
 if (!HasAuthority()) return;
 ProjectileMovement->Velocity = Direction.GetSafeNormal() * InitialSpeed;
}
void ASPArrowProjectile::OnArrowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 if (!HasAuthority() || !IsValid(OtherActor) || OtherActor == GetOwner() || OtherActor == this) return;
 const ASPCombatCharacter* Shooter = Cast<ASPCombatCharacter>(GetOwner());
 const ASPCombatCharacter* Target = Cast<ASPCombatCharacter>(OtherActor);
 if (Shooter && Target && Shooter->GetCombatTeam() != ESPTeam::Unassigned && Shooter->GetCombatTeam() == Target->GetCombatTeam()) { Destroy(); return; }
 UGameplayStatics::ApplyDamage(OtherActor, FMath::Max(0.f, Damage), GetInstigatorController(), this, UDamageType::StaticClass());
 Destroy();
}
void ASPArrowProjectile::OnArrowHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
 if (HasAuthority() && OtherActor != GetOwner()) Destroy();
}
