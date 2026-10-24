#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPProgressionTypes.h"
#include "SPProgressionWidget.generated.h"
class USPProgressionSubsystem;
UCLASS(Abstract, Blueprintable)
class SHADOWPROTOCOL_API USPProgressionWidget : public UUserWidget
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintPure, Category="HUD") int32 Coins() const;
 UFUNCTION(BlueprintPure, Category="HUD") int32 XP() const;
 UFUNCTION(BlueprintPure, Category="HUD") ESPRank Rank() const;
 UFUNCTION(BlueprintPure, Category="HUD") FName EquippedCar() const;
 UFUNCTION(BlueprintPure, Category="HUD") bool IsOwned(FName CarId) const;
 UFUNCTION(BlueprintCallable, Category="Garage") bool BuyCar(FName CarId);
 UFUNCTION(BlueprintCallable, Category="Garage") bool SelectCar(FName CarId);
 UFUNCTION(BlueprintImplementableEvent, Category="HUD") void RefreshProgression();
protected:
 virtual void NativeConstruct() override;
 virtual void NativeDestruct() override;
private:
 UFUNCTION() void HandleProgressionUpdated();
 USPProgressionSubsystem* Progress() const;
};
