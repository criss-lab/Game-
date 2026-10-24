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
 UPROPERTY(ReplicatedUsing=OnRep_Sprinting, BlueprintReadOnly, Category="Movement") bool bSprinting = false;
 UFUNCTION(BlueprintCallable, Category="Mobile") void MobileMove(FVector2D Axis);
 UFUNCTION(BlueprintCallable, Category="Mobile") void MobileLook(FVector2D Axis);
 UFUNCTION(BlueprintCallable, Category="Movement") void StartSprint();
 UFUNCTION(BlueprintCallable, Category="Movement") void StopSprint();
 UFUNCTION(BlueprintCallable, Category="Movement") void ToggleCrouch();
 UFUNCTION(BlueprintCallable, Category="Movement") void Dodge();
protected:
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera") TObjectPtr<USpringArmComponent> CameraBoom;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera") TObjectPtr<UCameraComponent> FollowCamera;\n UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(ClampMin="100.0")) float WalkSpeed = 420.f;\n UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(ClampMin="100.0")) float SprintSpeed = 650.f;\n UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(ClampMin="0.0")) float DodgeStrength = 720.f;\n UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement", meta=(ClampMin="0.1")) float DodgeCooldown = 1.0f;
 void MoveForward(float Value);
 void MoveRight(float Value);
 void LookYaw(float Value);
 void LookPitch(float Value);
 void StartJump();
 void StopJumpingInput();\n UFUNCTION(Server, Reliable) void ServerSetSprinting(bool bNewSprinting);\n UFUNCTION() void OnRep_Sprinting();\n virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;\n double LastDodgeTime = -1000.0;
};
