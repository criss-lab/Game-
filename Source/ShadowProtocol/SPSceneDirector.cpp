#include "SPSceneDirector.h"
#include "Net/UnrealNetwork.h"
ASPSceneDirector::ASPSceneDirector() { bReplicates = true; PrimaryActorTick.bCanEverTick = false; }
void ASPSceneDirector::AdvanceScene(ESPScene NextScene)
{
 if (!HasAuthority()) return;
 if (static_cast<uint8>(NextScene) != static_cast<uint8>(CurrentScene) + 1) return;
 CurrentScene = NextScene;
 OnRep_Scene();
 ForceNetUpdate();
}
void ASPSceneDirector::OnRep_Scene() { SceneChanged(CurrentScene); }
void ASPSceneDirector::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPSceneDirector, CurrentScene);
}
