#include "SPMobileControls.h"
#include "SPCombatCharacter.h"
#include "SPDriveableVehicle.h"
#include "SPDriveableHelicopter.h"
ASPCombatCharacter* USPMobileControls::LocalCharacter() const
{
 return Cast<ASPCombatCharacter>(GetOwningPlayerPawn());
}
void USPMobileControls::PressMelee()
{
 if (ASPCombatCharacter* Character = LocalCharacter()) Character->RequestMeleeAttack();
}
void USPMobileControls::PressInteract() { if (ASPCombatCharacter* C = LocalCharacter()) C->InteractWithWorld(); }
void USPMobileControls::PressArrowFire() { if (ASPCombatCharacter* C = LocalCharacter()) C->FireArrow(); }
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
 Axis = Axis.GetClampedToMaxSize(1.f);
 APawn* Pawn = GetOwningPlayerPawn();
 if (ASPCombatCharacter* Character = Cast<ASPCombatCharacter>(Pawn)) Character->MobileMove(Axis);
 else if (ASPDriveableVehicle* Vehicle = Cast<ASPDriveableVehicle>(Pawn)) Vehicle->SetMobileDriveInput(Axis);
 else if (ASPDriveableHelicopter* Helicopter = Cast<ASPDriveableHelicopter>(Pawn)) Helicopter->MobileMove(Axis);
}
void USPMobileControls::SetLookStick(FVector2D Axis)
{
 Axis = Axis.GetClampedToMaxSize(1.f);
 APawn* Pawn = GetOwningPlayerPawn();
 if (ASPCombatCharacter* Character = Cast<ASPCombatCharacter>(Pawn)) Character->MobileLook(Axis);
 else if (ASPDriveableHelicopter* Helicopter = Cast<ASPDriveableHelicopter>(Pawn)) Helicopter->MobileLook(Axis);
}

void USPMobileControls::PressExitVehicle()
{
 APawn* Pawn = GetOwningPlayerPawn();
 if (ASPDriveableVehicle* Vehicle = Cast<ASPDriveableVehicle>(Pawn)) Vehicle->ExitVehicle();
 else if (ASPDriveableHelicopter* Helicopter = Cast<ASPDriveableHelicopter>(Pawn)) Helicopter->ExitHelicopter();
}
