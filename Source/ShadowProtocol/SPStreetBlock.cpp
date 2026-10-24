#include "SPStreetBlock.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
ASPStreetBlock::ASPStreetBlock()
{
 PrimaryActorTick.bCanEverTick = false;
 SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
 SetRootComponent(SceneRoot);
 Road = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Road"));
 Road->SetupAttachment(SceneRoot);
 Sidewalks = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Sidewalks"));
 Sidewalks->SetupAttachment(SceneRoot);
 Buildings = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Buildings"));
 Buildings->SetupAttachment(SceneRoot);
}
void ASPStreetBlock::RebuildBlock()
{
 Road->ClearInstances();
 Sidewalks->ClearInstances();
 Buildings->ClearInstances();
 FRandomStream Random(CitySeed);
 for (int32 Index = 0; Index < BlockCount; ++Index)
 {
  const float X = Index * BlockLength;
  // Artist-provided source meshes should use a 100 cm reference footprint.
  Road->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X,0.f,0.f), FVector(BlockLength/100.f, 9.f, 0.15f)));
  for (int32 Side : {-1, 1})
  {
   const float Direction = static_cast<float>(Side);
   Sidewalks->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Direction*550.f, 15.f), FVector(BlockLength/100.f, 2.f, 0.25f)));
   const float Height = Random.FRandRange(6.f, 22.f);
   Buildings->AddInstance(FTransform(FRotator::ZeroRotator, FVector(X, Direction*1150.f, Height*50.f), FVector(7.f, 9.f, Height)));
  }
 }
}
