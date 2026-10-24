#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPMobileControls.generated.h"
class ASPCombatCharacter;
UCLASS(Abstract, Blueprintable)
class SHADOWPROTOCOL_API USPMobileControls : public UUserWidget
{
 GENERATED_BODY()
public:
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressMelee();
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressInteract();
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressArrowFire();
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressJump();
 UFUNCTION(BlueprintCallable, Category="Mobile") void ReleaseJump();
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressBlock();
 UFUNCTION(BlueprintCallable, Category="Mobile") void ReleaseBlock();
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressSprint();
 UFUNCTION(BlueprintCallable, Category="Mobile") void ReleaseSprint();
 UFUNCTION(BlueprintCallable, Category="Mobile") void PressDodge();
 UFUNCTION(BlueprintCallable, Category="Mobile") void ToggleCrouch();
 UFUNCTION(BlueprintCallable, Category="Mobile") void SetMoveStick(FVector2D Axis);
 UFUNCTION(BlueprintCallable, Category="Mobile") void SetLookStick(FVector2D Axis);
private:
 ASPCombatCharacter* LocalCharacter() const;
};
