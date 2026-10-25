#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPSceneDirector.h"
#include "SPCinematicDirector.generated.h"
class ULevelSequence;
class ULevelSequencePlayer;
UCLASS()
class SHADOWPROTOCOL_API ASPCinematicDirector : public AActor
{
 GENERATED_BODY()
public:
 ASPCinematicDirector();
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cinema") TObjectPtr<ASPSceneDirector> MissionDirector;
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cinema") TMap<ESPScene, TObjectPtr<ULevelSequence>> SceneSequences;
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cinema") bool bPlayOnSceneChange = true;
 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cinema") bool bEnableCinematics = true;
 UFUNCTION(BlueprintCallable, Category="Cinema") void PlayScene(ESPScene Scene);
 UFUNCTION(BlueprintCallable, Category="Cinema") void SkipCurrentScene();
 UFUNCTION(BlueprintImplementableEvent, Category="Cinema") void OnCinematicStarted(ESPScene Scene);
 UFUNCTION(BlueprintImplementableEvent, Category="Cinema") void OnCinematicFinished(ESPScene Scene);
protected:
 virtual void BeginPlay() override;
private:
 UFUNCTION() void HandleMissionScene(ESPScene Scene);
 UFUNCTION() void HandleSequenceFinished();
 UPROPERTY(Transient) TObjectPtr<ULevelSequencePlayer> ActivePlayer;
 ESPScene ActiveScene = ESPScene::Briefing;
};
