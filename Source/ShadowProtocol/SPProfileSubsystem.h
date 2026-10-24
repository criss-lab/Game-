#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPProfileSubsystem.generated.h"
UCLASS()
class SHADOWPROTOCOL_API USPProfileSubsystem : public UGameInstanceSubsystem
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintCallable, Category="Profile") bool LoadLocalProfile();
 UFUNCTION(BlueprintCallable, Category="Profile") bool SaveLocalProfile();
 UFUNCTION(BlueprintCallable, Category="Profile") bool SetLocalAlias(const FString& Alias);
 UFUNCTION(BlueprintPure, Category="Profile") FString GetLocalAlias() const;
 UFUNCTION(BlueprintCallable, Category="Profile") void RecordMissionCompleted(int32 MissionNumber);
 UFUNCTION(BlueprintPure, Category="Profile") int32 GetUnlockedMission() const;
};
