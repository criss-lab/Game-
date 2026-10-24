#include "SPProgressionSubsystem.h"
#include "SPOfflineSave.h"
#include "SPIdentityComponent.h"
#include "Kismet/GameplayStatics.h"
const FString USPProgressionSubsystem::Slot(TEXT("ShadowProtocolProgressV2"));
USPProgressionSubsystem::USPProgressionSubsystem()
{
 auto AddOffer = [this](const TCHAR* Id, const TCHAR* Name, int32 Price, int32 Mission)
 {
  FSPCarOffer Offer;
  Offer.Id = FName(Id);
  Offer.DisplayName = FText::FromString(Name);
  Offer.Price = Price;
  Offer.RequiredMission = Mission;
  CarCatalog.Add(Offer);
 };
 AddOffer(TEXT("STREET_COMPACT"), TEXT("Street Compact"), 250, 1);
 AddOffer(TEXT("CITY_SEDAN"), TEXT("City Sedan"), 800, 2);
 AddOffer(TEXT("TACTICAL_SUV"), TEXT("Tactical SUV"), 1800, 3);
 AddOffer(TEXT("SPORT_COUPE"), TEXT("Sport Coupe"), 3500, 4);
}

void USPProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
 Super::Initialize(Collection);
 LoadProgress();
}
USPOfflineSave* USPProgressionSubsystem::EnsureData()
{
 if (!Data) Data = Cast<USPOfflineSave>(UGameplayStatics::CreateSaveGameObject(USPOfflineSave::StaticClass()));
 return Data;
}
bool USPProgressionSubsystem::LoadProgress()
{
 if (!UGameplayStatics::DoesSaveGameExist(Slot, 0))
 {
  if (!EnsureData()) return false;
  const FString LegacySlot(TEXT("ShadowProtocolLocalV1"));
  if (UGameplayStatics::DoesSaveGameExist(LegacySlot, 0))
  {
   USPOfflineSave* Legacy = Cast<USPOfflineSave>(UGameplayStatics::LoadGameFromSlot(LegacySlot, 0));
   if (Legacy && Legacy->SaveVersion == 1)
   {
    Data->LocalAlias = USPIdentityComponent::IsValidAlias(Legacy->LocalAlias) ? Legacy->LocalAlias : TEXT("Ghost-0001");
    Data->CompletedMissions = FMath::Max(0, Legacy->CompletedMissions);
    Data->UnlockedMission = FMath::Max(1, Legacy->UnlockedMission);
    // Historical completion is migrated without retroactively granting currency or XP.
    for (int32 Mission = 1; Mission <= FMath::Min(Data->CompletedMissions, 1000); ++Mission) Data->RewardedMissions.AddUnique(Mission);
   }
  }
  return Commit();
 }
 USPOfflineSave* Loaded = Cast<USPOfflineSave>(UGameplayStatics::LoadGameFromSlot(Slot, 0));
 if (!Loaded || Loaded->SaveVersion != 2) return false;
 Data = Loaded;
 OnProgressionUpdated.Broadcast();
 return true;
}
bool USPProgressionSubsystem::SetAlias(const FString& Alias)
{
 if (!USPIdentityComponent::IsValidAlias(Alias) || !EnsureData()) return false;
 const FString Previous = Data->LocalAlias;
 Data->LocalAlias = Alias.TrimStartAndEnd();
 if (Commit()) return true;
 Data->LocalAlias = Previous;
 return false;
}
FString USPProgressionSubsystem::GetAlias() const { return Data ? Data->LocalAlias : FString(TEXT("Ghost-0001")); }
bool USPProgressionSubsystem::Commit()
{
 if (!EnsureData() || !UGameplayStatics::SaveGameToSlot(Data, Slot, 0)) return false;
 OnProgressionUpdated.Broadcast();
 return true;
}
bool USPProgressionSubsystem::CompleteMission(FSPMissionReward Reward)
{
 if (Reward.Mission < 1 || Reward.Coins < 0 || Reward.XP < 0 || !EnsureData()) return false;
 if (Reward.Mission > Data->UnlockedMission || Data->RewardedMissions.Contains(Reward.Mission)) return false;
 const int32 OldCoins = Data->Coins, OldXP = Data->Experience, OldCompleted = Data->CompletedMissions, OldUnlocked = Data->UnlockedMission;
 if (Reward.Coins > MAX_int32 - OldCoins || Reward.XP > MAX_int32 - OldXP) return false;
 Data->Coins += Reward.Coins;
 Data->Experience += Reward.XP;
 Data->CompletedMissions = FMath::Max(Data->CompletedMissions, Reward.Mission);
 Data->UnlockedMission = FMath::Max(Data->UnlockedMission, Reward.Mission + 1);
 Data->RewardedMissions.Add(Reward.Mission);
 if (Commit()) return true;
 Data->Coins = OldCoins; Data->Experience = OldXP; Data->CompletedMissions = OldCompleted; Data->UnlockedMission = OldUnlocked;
 Data->RewardedMissions.Remove(Reward.Mission);
 return false;
}
bool USPProgressionSubsystem::CollectPickup(FName UniquePickupId, int32 Amount)
{
 if (UniquePickupId.IsNone() || Amount < 1 || Amount > 1000 || !EnsureData() || Data->CollectedPickups.Contains(UniquePickupId) || Amount > MAX_int32 - Data->Coins) return false;
 Data->Coins += Amount;
 Data->CollectedPickups.Add(UniquePickupId);
 if (Commit()) return true;
 Data->Coins -= Amount; Data->CollectedPickups.Remove(UniquePickupId);
 return false;
}
bool USPProgressionSubsystem::PurchaseCar(FName CarId)
{
 if (CarId.IsNone() || !EnsureData() || OwnsCar(CarId)) return false;
 const FSPCarOffer* Offer = CarCatalog.FindByPredicate([CarId](const FSPCarOffer& Entry){ return Entry.Id == CarId; });
 if (!Offer || Offer->Price < 0 || Offer->RequiredMission > Data->UnlockedMission || Data->Coins < Offer->Price) return false;
 Data->Coins -= Offer->Price;
 Data->OwnedCars.Add(CarId);
 if (Commit()) return true;
 Data->OwnedCars.Remove(CarId); Data->Coins += Offer->Price;
 return false;
}
bool USPProgressionSubsystem::EquipCar(FName CarId)
{
 if (!OwnsCar(CarId) || !EnsureData()) return false;
 const FName Previous = Data->EquippedCar;
 Data->EquippedCar = CarId;
 if (Commit()) return true;
 Data->EquippedCar = Previous;
 return false;
}
int32 USPProgressionSubsystem::GetCoins() const { return Data ? Data->Coins : 0; }
int32 USPProgressionSubsystem::GetExperience() const { return Data ? Data->Experience : 0; }
ESPRank USPProgressionSubsystem::GetRank() const
{
 const int32 XP = GetExperience();
 return XP >= 10000 ? ESPRank::Legend : XP >= 5000 ? ESPRank::Elite : XP >= 2000 ? ESPRank::Specialist : XP >= 500 ? ESPRank::Operative : ESPRank::Recruit;
}
bool USPProgressionSubsystem::OwnsCar(FName CarId) const { return Data && !CarId.IsNone() && Data->OwnedCars.Contains(CarId); }
FName USPProgressionSubsystem::GetEquippedCar() const { return Data ? Data->EquippedCar : NAME_None; }
int32 USPProgressionSubsystem::GetUnlockedMission() const { return Data ? Data->UnlockedMission : 1; }
