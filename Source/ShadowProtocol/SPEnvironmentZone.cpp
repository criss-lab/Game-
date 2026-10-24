#include "SPEnvironmentZone.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
ASPEnvironmentZone::ASPEnvironmentZone()
{
 PrimaryActorTick.bCanEverTick = false;
 SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
 SetRootComponent(SceneRoot);
 Ground = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Ground"));
 Ground->SetupAttachment(SceneRoot);
 PrimaryInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("PrimaryInstances"));
 PrimaryInstances->SetupAttachment(SceneRoot);
 SecondaryInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("SecondaryInstances"));
 SecondaryInstances->SetupAttachment(SceneRoot);
}
void ASPEnvironmentZone::BuildEnvironment()
{
 Ground->ClearInstances(); PrimaryInstances->ClearInstances(); SecondaryInstances->ClearInstances();
 Ground->SetStaticMesh(GroundMesh); PrimaryInstances->SetStaticMesh(PrimaryMesh); SecondaryInstances->SetStaticMesh(SecondaryMesh);
 if (GroundMesh) Ground->AddInstance(FTransform(FRotator::ZeroRotator, FVector(0.f,0.f,-10.f), FVector(FMath::Max(1.f,Radius/50.f),FMath::Max(1.f,Radius/50.f),0.1f)));
 if (!PrimaryMesh && !SecondaryMesh) return;
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
    UniformScale = Random.FRandRange(0.8f,1.8f); break;
   case ESPEnvironmentBiome::Forest:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    if (FVector(Location.X,Location.Y,0.f).SizeSquared() > Radius*Radius) { --Index; continue; }
    UniformScale = Random.FRandRange(0.75f,1.45f); break;
   case ESPEnvironmentBiome::Suburb:
    Location = FVector((Index%10-5)*GridSpacing+Random.FRandRange(-150.f,150.f),(Index/10%10-5)*GridSpacing+Random.FRandRange(-150.f,150.f),0.f);
    UniformScale = Random.FRandRange(0.8f,1.2f); break;
   case ESPEnvironmentBiome::Industrial:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    UniformScale = Random.FRandRange(1.f,2.f); break;
   case ESPEnvironmentBiome::Desert:
   case ESPEnvironmentBiome::Coastal:
    Location = FVector(Random.FRandRange(-Radius,Radius),Random.FRandRange(-Radius,Radius),0.f);
    UniformScale = Random.FRandRange(0.6f,1.4f); break;
  }
  if (PrimaryMesh) PrimaryInstances->AddInstance(FTransform(Rotation,Location,FVector(UniformScale)));
  if (SecondaryMesh && Random.FRand() > 0.45f)
   SecondaryInstances->AddInstance(FTransform(Rotation,Location+FVector(Random.FRandRange(-350.f,350.f),Random.FRandRange(-350.f,350.f),0.f),FVector(Random.FRandRange(0.5f,1.2f))));
 }
}