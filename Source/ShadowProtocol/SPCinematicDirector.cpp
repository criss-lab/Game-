#include "SPCinematicDirector.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
ASPCinematicDirector::ASPCinematicDirector()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = false; // Each local client owns presentation; mission state remains authoritative.
}
void ASPCinematicDirector::BeginPlay()
{
 Super::BeginPlay();
 if (MissionDirector)
 {
  MissionDirector->OnSceneChangedNative.AddUObject(this, &ASPCinematicDirector::HandleMissionScene);
  if (bPlayOnSceneChange) HandleMissionScene(MissionDirector->CurrentScene);
 }
}
void ASPCinematicDirector::HandleMissionScene(ESPScene Scene)
{
 if (bPlayOnSceneChange) PlayScene(Scene);
}
void ASPCinematicDirector::PlayScene(ESPScene Scene)
{
 if (!bEnableCinematics) return;
 ULevelSequence* const* Sequence = SceneSequences.Find(Scene);
 if (!Sequence || !*Sequence || !GetWorld()) return;
 SkipCurrentScene();
 ActiveScene = Scene;
 FMovieSceneSequencePlaybackSettings Settings;
 ALevelSequenceActor* SequenceActor = nullptr;
 ActivePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), *Sequence, Settings, SequenceActor);
 if (!ActivePlayer) return;
 ActivePlayer->OnFinished.AddDynamic(this, &ASPCinematicDirector::HandleSequenceFinished);
 OnCinematicStarted(Scene);
 ActivePlayer->Play();
}
void ASPCinematicDirector::SkipCurrentScene()
{
 if (!ActivePlayer) return;
 ActivePlayer->OnFinished.RemoveDynamic(this, &ASPCinematicDirector::HandleSequenceFinished);
 ActivePlayer->Stop();
 ActivePlayer = nullptr;
 OnCinematicFinished(ActiveScene);
}
void ASPCinematicDirector::HandleSequenceFinished()
{
 if (!ActivePlayer) return;
 ActivePlayer->OnFinished.RemoveDynamic(this, &ASPCinematicDirector::HandleSequenceFinished);
 ActivePlayer = nullptr;
 OnCinematicFinished(ActiveScene);
}
