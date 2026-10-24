#include "SPMissionRewards.h"
#include "SPProgressionSubsystem.h"
#include "Engine/GameInstance.h"
ASPMissionRewards::ASPMissionRewards() { PrimaryActorTick.bCanEverTick = false; bReplicates = false; }
void ASPMissionRewards::BeginPlay()
{
 Super::BeginPlay();
 if (Director) BindDirector(Director);
}
void ASPMissionRewards::BindDirector(ASPSceneDirector* InDirector)
{
 if (Director) Director->OnSceneChangedNative.RemoveAll(this);
 Director = InDirector;
 if (!Director) return;
 Director->OnSceneChangedNative.AddUObject(this, &ASPMissionRewards::HandleScene);
 HandleScene(Director->CurrentScene);
}
void ASPMissionRewards::HandleScene(ESPScene Scene)
{
 // Local solo game only: online awards require authenticated server-side validation.
 if (Scene != ESPScene::Debrief || GetNetMode() != NM_Standalone) return;
 UGameInstance* Instance = GetGameInstance();
 USPProgressionSubsystem* Progress = Instance ? Instance->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 if (Progress && Progress->CompleteMission(Reward)) OnRewardGranted(Reward);
}
