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
void USPMobileControls::PressBlock() { if (ASPCombatCharacter* C = LocalCharacter()) C->StartBlock(); }
void USPMobileControls::ReleaseBlock() { if (ASPCombatCharacter* C = LocalCharacter()) C->StopBlock(); }
void USPMobileControls::PressSprint() { if (ASPCombatCharacter* C = LocalCharacter()) C->StartSprint(); }
void USPMobileControls::ReleaseSprint() { if (ASPCombatCharacter* C = LocalCharacter()) C->StopSprint(); }
void USPMobileControls::PressDodge() { if (ASPCombatCharacter* C = LocalCharacter()) C->Dodge(); }
void USPMobileControls::ToggleCrouch() { if (ASPCombatCharacter* C = LocalCharacter()) C->ToggleCrouch(); }
void USPMobileControls::SetMoveStick(FVector2D Axis)
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->MobileMove(Axis);
}
void USPMobileControls::SetLookStick(FVector2D Axis)
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->MobileLook(Axis);
}
