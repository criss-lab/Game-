#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPEnvironmentZone.generated.h"
class UInstancedStaticMeshComponent;
class UStaticMesh;
UENUM(BlueprintType)
enum class ESPEnvironmentBiome : uint8 { City, Forest, Suburb, Industrial, Desert, Coastal };
UCLASS()
class SHADOWPROTOCOL_API ASPEnvironmentZone : public AActor
{
 GENERATED_BODY()
public:
 ASPEnvironmentZone();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Environment") TObjectPtr<USceneComponent> SceneRoot;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Environment") TObjectPtr<UInstancedStaticMeshComponent> Ground;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Environment") TObjectPtr<UInstancedStaticMeshComponent> PrimaryInstances;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Environment") TObjectPtr<UInstancedStaticMeshComponent> SecondaryInstances;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment") ESPEnvironmentBiome Biome = ESPEnvironmentBiome::City;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment", meta=(ClampMin="500.0")) float Radius = 5000.f;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment", meta=(ClampMin="0", ClampMax="500")) int32 ScatterCount = 100;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment") int32 Seed = 2050;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment") TObjectPtr<UStaticMesh> GroundMesh;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment") TObjectPtr<UStaticMesh> PrimaryMesh;
 UPROPERTY(ReplicatedUsing=OnRep_EnvironmentSettings, EditAnywhere, BlueprintReadWrite, Category="Environment") TObjectPtr<UStaticMesh> SecondaryMesh;
 UPROPERTY() TObjectPtr<UStaticMesh> FallbackPlaneMesh;
 UPROPERTY() TObjectPtr<UStaticMesh> FallbackCubeMesh;
 UPROPERTY() TObjectPtr<UStaticMesh> FallbackSphereMesh;
 UPROPERTY() TObjectPtr<UStaticMesh> FallbackConeMesh;
 UPROPERTY() TObjectPtr<UStaticMesh> FallbackCylinderMesh;
 UFUNCTION(CallInEditor, BlueprintCallable, Category="Environment") void BuildEnvironment();
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
 virtual void BeginPlay() override;
private:
 UFUNCTION() void OnRep_EnvironmentSettings();
};