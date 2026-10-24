#include "SPProfileSubsystem.h"
#include "SPProgressionSubsystem.h"
#include "Engine/GameInstance.h"
bool USPProfileSubsystem::LoadLocalProfile()
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress && Progress->LoadProgress();
}
bool USPProfileSubsystem::SaveLocalProfile()
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress && Progress->SaveProgress();
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
 // Legacy compatibility only updates mission progress; it never grants rewards.
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 if (Progress) Progress->RecordLegacyMissionProgress(MissionNumber);
}
int32 USPProfileSubsystem::GetUnlockedMission() const
{
 auto* Progress = GetGameInstance() ? GetGameInstance()->GetSubsystem<USPProgressionSubsystem>() : nullptr;
 return Progress ? Progress->GetUnlockedMission() : 1;
}
