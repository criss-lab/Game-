#include "SPProgressionWidget.h"
#include "SPProgressionSubsystem.h"
#include "Engine/GameInstance.h"
USPProgressionSubsystem* USPProgressionWidget::Progress() const
{
 UGameInstance* Instance = GetGameInstance();
 return Instance ? Instance->GetSubsystem<USPProgressionSubsystem>() : nullptr;
}
void USPProgressionWidget::NativeConstruct()
{
 Super::NativeConstruct();
 if (USPProgressionSubsystem* P = Progress())
 {
  P->OnProgressionUpdated.AddUniqueDynamic(this, &USPProgressionWidget::HandleProgressionUpdated);
  RefreshProgression();
 }
}
void USPProgressionWidget::NativeDestruct()
{
 if (USPProgressionSubsystem* P = Progress()) P->OnProgressionUpdated.RemoveDynamic(this, &USPProgressionWidget::HandleProgressionUpdated);
 Super::NativeDestruct();
}
void USPProgressionWidget::HandleProgressionUpdated() { RefreshProgression(); }
int32 USPProgressionWidget::Coins() const { const auto* P = Progress(); return P ? P->GetCoins() : 0; }
int32 USPProgressionWidget::XP() const { const auto* P = Progress(); return P ? P->GetExperience() : 0; }
ESPRank USPProgressionWidget::Rank() const { const auto* P = Progress(); return P ? P->GetRank() : ESPRank::Recruit; }
FName USPProgressionWidget::EquippedCar() const { const auto* P = Progress(); return P ? P->GetEquippedCar() : NAME_None; }
bool USPProgressionWidget::IsOwned(FName Id) const { const auto* P = Progress(); return P && P->OwnsCar(Id); }
bool USPProgressionWidget::BuyCar(FName Id) { auto* P = Progress(); return P && P->PurchaseCar(Id); }
bool USPProgressionWidget::SelectCar(FName Id) { auto* P = Progress(); return P && P->EquipCar(Id); }
