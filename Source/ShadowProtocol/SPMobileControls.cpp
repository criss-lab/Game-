#include "SPMobileControls.h"
#include "SPCombatCharacter.h"
#include "Kismet/GameplayStatics.h"
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
