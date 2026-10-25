#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPSceneDirector.generated.h"
UENUM(BlueprintType)
enum class ESPScene : uint8 { Briefing, Infiltration, Ambush, BossFight, VehicleEscape, Debrief };
DECLARE_MULTICAST_DELEGATE_OneParam(FSPSceneChangedNative, ESPScene);
UCLASS()
class SHADOWPROTOCOL_API ASPSceneDirector : public AActor
{
 GENERATED_BODY()
public:
 ASPSceneDirector();
 FSPSceneChangedNative OnSceneChangedNative;
 UPROPERTY(ReplicatedUsing=OnRep_Scene, BlueprintReadOnly, Category="Mission") ESPScene CurrentScene = ESPScene::Briefing;
 UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Mission") void AdvanceScene(ESPScene NextScene);
 UFUNCTION(BlueprintImplementableEvent, Category="Mission") void SceneChanged(ESPScene NewScene);
 UFUNCTION() void OnRep_Scene();
 virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
