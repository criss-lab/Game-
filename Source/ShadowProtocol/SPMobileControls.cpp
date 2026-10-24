#include "SPMobileControls.h"
#include "SPCombatCharacter.h"
ASPCombatCharacter* USPMobileControls::LocalCharacter() const
{
 return Cast<ASPCombatCharacter>(GetOwningPlayerPawn());
}
void USPMobileControls::PressMelee()
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->RequestMeleeAttack();
}
void USPMobileControls::PressJump()
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->Jump();
}
void USPMobileControls::ReleaseJump()
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->StopJumping();
}
void USPMobileControls::SetMoveStick(FVector2D Axis)
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->MobileMove(Axis);
}
void USPMobileControls::SetLookStick(FVector2D Axis)
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->MobileLook(Axis);
}
