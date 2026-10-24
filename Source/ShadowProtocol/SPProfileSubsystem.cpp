#include "SPProfileSubsystem.h"
#include "SPProgressionSubsystem.h"
#include "Engine/GameInstance.h"
const FString USPProfileSubsystem::SlotName(TEXT("ShadowProtocolLocalV1"));
USPOfflineSave* USPProfileSubsystem::EnsureProfile() { return nullptr; }
bool USPProfileSubsystem::LoadLocalProfile()
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress && Progress->LoadProgress();
}
bool USPProfileSubsystem::SaveLocalProfile()
{
 // Writes are performed transactionally by the unified progression subsystem.
 return GetGameInstance() && GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() != nullptr;
}
bool USPProfileSubsystem::SetLocalAlias(const FString& Alias)
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress && Progress->SetAlias(Alias);
}
FString USPProfileSubsystem::GetLocalAlias() const
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress ? Progress->GetAlias() : FString(TEXT("Ghost-0001"));
}
void USPProfileSubsystem::RecordMissionCompleted(int32 MissionNumber)
{
 // Deprecated: only validated offline debrief via SPMissionRewards can award completion.
}
int32 USPProfileSubsystem::GetUnlockedMission() const
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress ? Progress->GetUnlockedMission() : 1;
}
