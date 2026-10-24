#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPProgressionTypes.h"
#include "SPProgressionSubsystem.generated.h"
class USPOfflineSave;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSPProgressionUpdated);
UCLASS()
class SHADOWPROTOCOL_API USPProgressionSubsystem : public UGameInstanceSubsystem
{
 GENERATED_BODY()
public:
 UPROPERTY(BlueprintAssignable, Category="Progression") FSPProgressionUpdated OnProgressionUpdated;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Garage") TArray<FSPCarOffer> CarCatalog;
 UFUNCTION(BlueprintCallable, Category="Progression") bool LoadProgress();
 UFUNCTION(BlueprintCallable, Category="Progression") bool CompleteMission(FSPMissionReward Reward);
 UFUNCTION(BlueprintCallable, Category="Progression") bool CollectPickup(FName UniquePickupId, int32 Amount);
 UFUNCTION(BlueprintCallable, Category="Garage") bool PurchaseCar(FName CarId);
 UFUNCTION(BlueprintCallable, Category="Garage") bool EquipCar(FName CarId);
 UFUNCTION(BlueprintPure, Category="Progression") int32 GetCoins() const;
 UFUNCTION(BlueprintPure, Category="Progression") int32 GetExperience() const;
 UFUNCTION(BlueprintPure, Category="Progression") ESPRank GetRank() const;
 UFUNCTION(BlueprintPure, Category="Garage") bool OwnsCar(FName CarId) const;
 UFUNCTION(BlueprintPure, Category="Garage") FName GetEquippedCar() const;
 UFUNCTION(BlueprintPure, Category="Progression") int32 GetUnlockedMission() const;
private:
 UPROPERTY(Transient) TObjectPtr<USPOfflineSave> Data;
 USPOfflineSave* EnsureData();
 bool Commit();
 static const FString Slot;
};
