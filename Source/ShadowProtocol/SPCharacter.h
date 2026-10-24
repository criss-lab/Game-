#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SPCharacter.generated.h"
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class SHADOWPROTOCOL_API ASPCharacter : public ACharacter
{
 GENERATED_BODY()
public:
 ASPCharacter();
 virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
protected:
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera") TObjectPtr<USpringArmComponent> CameraBoom;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera") TObjectPtr<UCameraComponent> FollowCamera;
 void MoveForward(float Value);
 void MoveRight(float Value);
 void LookYaw(float Value);
 void LookPitch(float Value);
 void StartJump();
 void StopJumpingInput();
};
