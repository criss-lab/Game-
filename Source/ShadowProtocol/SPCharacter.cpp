#include "SPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
ASPCharacter::ASPCharacter()
{
 PrimaryActorTick.bCanEverTick = false;\n bReplicates = true;
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
void ASPCharacter::StartSprint()
{
 if (GetCharacterMovement() && !bIsCrouched) GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void ASPCharacter::StopSprint()
{
 if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = bIsCrouched ? WalkSpeed * 0.55f : WalkSpeed;
}
void ASPCharacter::ToggleCrouch()
{
 if (bIsCrouched) { UnCrouch(); StopSprint(); } else { StopSprint(); Crouch(); if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * 0.55f; }
}
void ASPCharacter::Dodge()
{
 if (!IsLocallyControlled() || !GetCharacterMovement() || GetCharacterMovement()->IsFalling()) return;
 const double Now = GetWorld()->GetTimeSeconds();
 if (Now - LastDodgeTime < DodgeCooldown) return;
 LastDodgeTime = Now;
 FVector Direction = GetLastMovementInputVector();
 Direction.Z = 0.f;
 if (Direction.IsNearlyZero()) Direction = GetActorForwardVector();
 LaunchCharacter(Direction.GetSafeNormal() * DodgeStrength + FVector(0,0,80.f), true, true);
}
void ASPCharacter::StopJumpingInput() { StopJumping(); }
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
