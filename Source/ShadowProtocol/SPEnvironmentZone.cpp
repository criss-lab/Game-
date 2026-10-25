#include "SPEnvironmentZone.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
ASPEnvironmentZone::ASPEnvironmentZone()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = true;
 SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
 SetRootComponent(SceneRoot);
 Ground = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Ground"));
 Ground->SetupAttachment(SceneRoot);
 PrimaryInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PrimaryInstances"));
 PrimaryInstances->SetupAttachment(SceneRoot);
 SecondaryInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SecondaryInstances"));
 SecondaryInstances->SetupAttachment(SceneRoot);
 static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
 static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
 static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
 static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeAsset(TEXT("/Engine/BasicShapes/Cone.Cone"));
 static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
 if (PlaneAsset.Succeeded()) FallbackPlaneMesh = PlaneAsset.Object;
 if (CubeAsset.Succeeded()) FallbackCubeMesh = CubeAsset.Object;
 if (SphereAsset.Succeeded()) FallbackSphereMesh = SphereAsset.Object;
 if (ConeAsset.Succeeded()) FallbackConeMesh = ConeAsset.Object;
 if (CylinderAsset.Succeeded()) FallbackCylinderMesh = CylinderAsset.Object;
}
void ASPEnvironmentZone::BeginPlay()
{
 Super::BeginPlay();
 BuildEnvironment();
}
void ASPEnvironmentZone::OnRep_EnvironmentSettings() { BuildEnvironment(); }
void ASPEnvironmentZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPEnvironmentZone, Biome);
 DOREPLIFETIME(ASPEnvironmentZone, Radius);
 DOREPLIFETIME(ASPEnvironmentZone, ScatterCount);
 DOREPLIFETIME(ASPEnvironmentZone, Seed);
 DOREPLIFETIME(ASPEnvironmentZone, GroundMesh);
 DOREPLIFETIME(ASPEnvironmentZone, PrimaryMesh);
 DOREPLIFETIME(ASPEnvironmentZone, SecondaryMesh);
}
void ASPEnvironmentZone::BuildEnvironment()
{
 Ground->ClearInstances(); PrimaryInstances->ClearInstances(); SecondaryInstances->ClearInstances();
 Ground->SetStaticMesh(GroundMesh ? GroundMesh : FallbackPlaneMesh);
 UStaticMesh* ChosenPrimary = PrimaryMesh;
 UStaticMesh* ChosenSecondary = SecondaryMesh;
 if (!ChosenPrimary)
 {
  switch (Biome)
  {
   case ESPEnvironmentBiome::Forest: ChosenPrimary = FallbackConeMesh; break;
   case ESPEnvironmentBiome::Desert: ChosenPrimary = FallbackSphereMesh; break;
   case ESPEnvironmentBiome::Coastal: ChosenPrimary = FallbackCylinderMesh; break;
   default: ChosenPrimary = FallbackCubeMesh; break;
  }
 }
 if (!ChosenSecondary)
 {
  switch (Biome)
  {
   case ESPEnvironmentBiome::Forest: ChosenSecondary = FallbackSphereMesh; break;
   case ESPEnvironmentBiome::Desert: ChosenSecondary = FallbackConeMesh; break;
   default: ChosenSecondary = FallbackCylinderMesh; break;
  }
 }
 PrimaryInstances->SetStaticMesh(ChosenPrimary);
 SecondaryInstances->SetStaticMesh(ChosenSecondary);
 if (Ground->GetStaticMesh()) Ground->AddInstance(FTransform(FRotator::ZeroRotator, FVector(0.f,0.f,-10.f), FVector(FMath::Max(1.f,Radius/50.f),FMath::Max(1.f,Radius/50.f),0.1f)));
 if (!ChosenPrimary && !ChosenSecondary) return;
 FRandomStream Random(Seed);
 const int32 SafeCount = FMath::Clamp(ScatterCount, 0, 500);
 const float GridSpacing = FMath::Max(900.f, Radius * 2.f / FMath::Sqrt(static_cast<float>(FMath::Max(1, SafeCount))));
 for (int32 Index=0; Index<SafeCount; ++Index)
 {
  FVector Location = FVector::ZeroVector;
  float UniformScale = 1.f;
  const FRotator Rotation(0.f, Random.FRandRange(0.f,360.f), 0.f);
  switch (Biome)
  {
   case ESPEnvironmentBiome::City:
    Location = FVector(Random.FRandRange(-Radius,Radius),(Index%2 ? 1.f : -1.f)*Random.FRandRange(Radius*0.45f,Radius),0.f);
    UniformScale = Random.FRandRange(8.f,22.f); break;
   case ESPEnvironmentBiome::Forest:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    if (FVector(Location.X,Location.Y,0.f).SizeSquared() > Radius*Radius) { --Index; continue; }
    UniformScale = Random.FRandRange(3.f,8.f); break;
   case ESPEnvironmentBiome::Suburb:
    Location = FVector((Index%10-5)*GridSpacing+Random.FRandRange(-150.f,150.f),(Index/10%10-5)*GridSpacing+Random.FRandRange(-150.f,150.f),0.f);
    UniformScale = Random.FRandRange(4.f,9.f); break;
   case ESPEnvironmentBiome::Industrial:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    UniformScale = Random.FRandRange(6.f,15.f); break;
   case ESPEnvironmentBiome::Desert:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    UniformScale = Random.FRandRange(1.5f,5.f); break;
   case ESPEnvironmentBiome::Coastal:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    UniformScale = Random.FRandRange(2.f,6.f); break;
  }
  if (ChosenPrimary) PrimaryInstances->AddInstance(FTransform(Rotation,Location,FVector(UniformScale)));
  if (ChosenSecondary && Random.FRand() > 0.45f)
   SecondaryInstances->AddInstance(FTransform(Rotation,Location+FVector(Random.FRandRange(-350.f,350.f),Random.FRandRange(-350.f,350.f),0.f),FVector(Random.FRandRange(0.5f,1.2f)*UniformScale*0.35f)));
 }
}
