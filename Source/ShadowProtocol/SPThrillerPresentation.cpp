#include "SPThrillerPresentation.h"
ASPThrillerPresentation::ASPThrillerPresentation()
{
 PrimaryActorTick.bCanEverTick = false;
 auto Add = [this](ESPScene Scene, const TCHAR* Title, const TCHAR* Objective, const TCHAR* Radio)
 {
  FSPSceneBeat Beat;
  Beat.Scene = Scene;
  Beat.ChapterTitle = FText::FromString(Title);
  Beat.ObjectiveLine = FText::FromString(Objective);
  Beat.RadioLine = FText::FromString(Radio);
  Beats.Add(Beat);
 };
 Add(ESPScene::Briefing, TEXT("THE BLACK CONVOY"), TEXT("Identify the convoy route"), TEXT("Control: Keep it quiet. We need answers, not headlines."));
 Add(ESPScene::Infiltration, TEXT("NO WITNESSES"), TEXT("Reach the observation point"), TEXT("Ghost: Then give me a way in."));
 Add(ESPScene::Ambush, TEXT("THE TRAP"), TEXT("Survive the ambush"), TEXT("Control: They knew you were coming."));
 Add(ESPScene::BossFight, TEXT("THE ENFORCER"), TEXT("Defeat the convoy commander"), TEXT("Ghost: We finish this here."));
 Add(ESPScene::VehicleEscape, TEXT("LAST EXIT"), TEXT("Escape the pursuit"), TEXT("Control: Roadblock ahead. Make your own exit."));
 Add(ESPScene::Debrief, TEXT("THE COST"), TEXT("Review the operation"), TEXT("Ghost: The convoy was bait. Find out who sent it."));
}
void ASPThrillerPresentation::BeginPlay()
{
 Super::BeginPlay();
 if (Director) BindDirector(Director);
}
void ASPThrillerPresentation::BindDirector(ASPSceneDirector* InDirector)
{
 if (Director) Director->OnSceneChangedNative.RemoveAll(this);
 Director = InDirector;
 if (!Director) return;
 Director->OnSceneChangedNative.AddUObject(this, &ASPThrillerPresentation::OnScene);
 OnScene(Director->CurrentScene);
}
void ASPThrillerPresentation::OnScene(ESPScene Scene)
{
 const FSPSceneBeat* Beat = Beats.FindByPredicate([Scene](const FSPSceneBeat& Entry){ return Entry.Scene == Scene; });
 if (Beat) PresentBeat(*Beat);
}
