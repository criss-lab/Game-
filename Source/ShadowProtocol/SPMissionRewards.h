#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPProgressionTypes.h"
#include "SPSceneDirector.h"
#include "SPMissionRewards.generated.h"
UCLASS()
class SHADOWPROTOCOL_API ASPMissionRewards : public AActor
{
 GENERATED_BODY()
public:
 ASPMissionRewards();
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission") TObjectPtr<ASPSceneDirector> Director;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission") FSPMissionReward Reward;
 UFUNCTION(BlueprintCallable, Category="Mission") void BindDirector(ASPSceneDirector* InDirector);
 UFUNCTION(BlueprintImplementableEvent, Category="Mission") void OnRewardGranted(FSPMissionReward Granted);
protected:
 virtual void BeginPlay() override;
private:
 void HandleScene(ESPScene Scene);
};
