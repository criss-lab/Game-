#include "SPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"

ASPCharacter::ASPCharacter()
{
 PrimaryActorTick.bCanEverTick = false;
 bReplicates = true;
 bUseControllerRotationYaw = false;
 GetCharacterMovement()->bOrientRotationToMovement = true;
 GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
 GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
 GetCharacterMovement()->MaxAcceleration = 1800.f;
 GetCharacterMovement()->BrakingDecelerationWalking = 1800.f;
 GetCharacterMovement()->GroundFriction = 8.f;
 GetCharacterMovement()->AirControl = 0.35f;
 GetCharacterMovement()->JumpZVelocity = 520.f;
 GetCharacterMovement()->BrakingDecelerationFalling = 1200.f;
 GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
 CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
 CameraBoom->SetupAttachment(RootComponent);
 CameraBoom->TargetArmLength = 320.f;
 CameraBoom->bEnableCameraLag = true;
 CameraBoom->CameraLagSpeed = 12.f;
 CameraBoom->bUsePawnControlRotation = true;
 FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
 FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
 FollowCamera->bUsePawnControlRotation = false;
}
void ASPCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
 Super::SetupPlayerInputComponent(Input);
 check(Input);
 Input->BindAxis(TEXT("MoveForward"), this, &ASPCharacter::MoveForward);
 Input->BindAxis(TEXT("MoveRight"), this, &ASPCharacter::MoveRight);
 Input->BindAxis(TEXT("Turn"), this, &ASPCharacter::LookYaw);
 Input->BindAxis(TEXT("LookUp"), this, &ASPCharacter::LookPitch);
 Input->BindAction(TEXT("Jump"), IE_Pressed, this, &ASPCharacter::StartJump);
 Input->BindAction(TEXT("Jump"), IE_Released, this, &ASPCharacter::StopJumpingInput);
 Input->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASPCharacter::StartSprint);
 Input->BindAction(TEXT("Sprint"), IE_Released, this, &ASPCharacter::StopSprint);
 Input->BindAction(TEXT("Crouch"), IE_Pressed, this, &ASPCharacter::ToggleCrouch);
 Input->BindAction(TEXT("Dodge"), IE_Pressed, this, &ASPCharacter::Dodge);
}
void ASPCharacter::MoveForward(float Value)
{
 if (!Controller || FMath::IsNearlyZero(Value)) return;
 const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
 AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
}
void ASPCharacter::MoveRight(float Value)
{
 if (!Controller || FMath::IsNearlyZero(Value)) return;
 const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
 AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
}
void ASPCharacter::LookYaw(float Value) { AddControllerYawInput(Value); }
void ASPCharacter::LookPitch(float Value) { AddControllerPitchInput(Value); }
void ASPCharacter::StartJump() { if (CanJump()) Jump(); }
void ASPCharacter::StopJumpingInput() { StopJumping(); }
void ASPCharacter::StartSprint()
{
 if (bIsCrouched) return;
 bSprinting = true;
 OnRep_Sprinting();
 if (HasAuthority()) ServerSetSprinting_Implementation(true);
 else ServerSetSprinting(true);
}
void ASPCharacter::StopSprint()
{
 bSprinting = false;
 OnRep_Sprinting();
 if (HasAuthority()) ServerSetSprinting_Implementation(false);
 else ServerSetSprinting(false);
}
void ASPCharacter::ServerSetSprinting_Implementation(bool bNewSprinting)
{
 bSprinting = bNewSprinting && !bIsCrouched;
 OnRep_Sprinting();
 ForceNetUpdate();
}
void ASPCharacter::OnRep_Sprinting()
{
 if (GetCharacterMovement())
  GetCharacterMovement()->MaxWalkSpeed = bIsCrouched ? WalkSpeed * 0.55f : (bSprinting ? SprintSpeed : WalkSpeed);
}
void ASPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
 Super::GetLifetimeReplicatedProps(OutLifetimeProps);
 DOREPLIFETIME(ASPCharacter, bSprinting);
}
void ASPCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
 Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
 if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.55f;
}
void ASPCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
 Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
 if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
}
void ASPCharacter::ToggleCrouch()
{
 if (bIsCrouched)
 {
  UnCrouch();
  StopSprint();
 }
 else
 {
  StopSprint();
  Crouch();
  if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.55f;
 }
}
void ASPCharacter::Dodge()
{
 if (!IsLocallyControlled() || !GetCharacterMovement() || GetCharacterMovement()->IsFalling() || !GetWorld()) return;
 const double Now = GetWorld()->GetTimeSeconds();
 if (Now - LastDodgeTime < DodgeCooldown) return;
 LastDodgeTime = Now;
 FVector Direction = GetLastMovementInputVector();
 Direction.Z = 0.f;
 if (Direction.IsNearlyZero()) Direction = GetActorForwardVector();
 LaunchCharacter(Direction.GetSafeNormal() * DodgeStrength + FVector(0.f, 0.f, 80.f), true, true);
}
void ASPCharacter::MobileMove(FVector2D Axis)
{
 if (!IsLocallyControlled()) return;
 Axis = Axis.GetClampedToMaxSize(1.f);
 MoveForward(Axis.Y);
 MoveRight(Axis.X);
}
void ASPCharacter::MobileLook(FVector2D Axis)
{
 if (!IsLocallyControlled()) return;
 Axis = Axis.GetClampedToMaxSize(1.f);
 LookYaw(Axis.X);
 LookPitch(Axis.Y);
}
