#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPStreetBlock.generated.h"
class UInstancedStaticMeshComponent;
UCLASS()
class SHADOWPROTOCOL_API ASPStreetBlock : public AActor
{
 GENERATED_BODY()
public:
 ASPStreetBlock();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="City") TObjectPtr<USceneComponent> SceneRoot;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="City") TObjectPtr<UInstancedStaticMeshComponent> Road;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="City") TObjectPtr<UInstancedStaticMeshComponent> Sidewalks;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="City") TObjectPtr<UInstancedStaticMeshComponent> Buildings;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="City", meta=(ClampMin="1", ClampMax="24")) int32 BlockCount = 6;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="City", meta=(ClampMin="500.0")) float BlockLength = 1800.f;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="City") int32 CitySeed = 2026;
 UFUNCTION(CallInEditor, BlueprintCallable, Category="City") void RebuildBlock();
};
