#include "SPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
ASPCharacter::ASPCharacter()
{
 PrimaryActorTick.bCanEverTick = false;
 bUseControllerRotationYaw = false;
 GetCharacterMovement()->bOrientRotationToMovement = true;
 CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
 CameraBoom->SetupAttachment(RootComponent);
 CameraBoom->TargetArmLength = 350.f;
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
void ASPCharacter::StartJump() { Jump(); }
void ASPCharacter::StopJumpingInput() { StopJumping(); }
