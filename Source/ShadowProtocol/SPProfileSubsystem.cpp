#include "SPProfileSubsystem.h"
#include "SPIdentityComponent.h"
#include "Kismet/GameplayStatics.h"
const FString USPProfileSubsystem::SlotName(TEXT("ShadowProtocolLocalV1"));
USPOfflineSave* USPProfileSubsystem::EnsureProfile()
{
 if (!Profile) Profile = Cast<USPOfflineSave>(UGameplayStatics::CreateSaveGameObject(USPOfflineSave::StaticClass()));
 return Profile;
}
bool USPProfileSubsystem::LoadLocalProfile()
{
 if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) return EnsureProfile() != nullptr;
 USPOfflineSave* Loaded = Cast<USPOfflineSave>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
 if (!Loaded || Loaded->SaveVersion != 1) return false;
 Profile = Loaded;
 return true;
}
bool USPProfileSubsystem::SaveLocalProfile()
{
 return EnsureProfile() && UGameplayStatics::SaveGameToSlot(Profile, SlotName, 0);
}
bool USPProfileSubsystem::SetLocalAlias(const FString& Alias)
{
 if (!USPIdentityComponent::IsValidAlias(Alias) || !EnsureProfile()) return false;
 Profile->LocalAlias = Alias.TrimStartAndEnd();
 return SaveLocalProfile();
}
FString USPProfileSubsystem::GetLocalAlias() const
{
 return Profile ? Profile->LocalAlias : FString(TEXT("Ghost-0001"));
}
void USPProfileSubsystem::RecordMissionCompleted(int32 MissionNumber)
{
 if (MissionNumber < 1 || !EnsureProfile()) return;
 Profile->CompletedMissions = FMath::Max(Profile->CompletedMissions, MissionNumber);
 Profile->UnlockedMission = FMath::Max(Profile->UnlockedMission, MissionNumber + 1);
 SaveLocalProfile();
}
int32 USPProfileSubsystem::GetUnlockedMission() const { return Profile ? Profile->UnlockedMission : 1; }
