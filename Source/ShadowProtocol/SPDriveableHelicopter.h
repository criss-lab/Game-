#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SPDriveableHelicopter.generated.h"
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
UCLASS()
class SHADOWPROTOCOL_API ASPDriveableHelicopter : public APawn
{
 GENERATED_BODY()
public:
 ASPDriveableHelicopter();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Helicopter") TObjectPtr<UStaticMeshComponent> HelicopterMesh;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Helicopter") TObjectPtr<USpringArmComponent> CameraBoom;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Helicopter") TObjectPtr<UCameraComponent> PilotCamera;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Helicopter") TObjectPtr<UFloatingPawnMovement> FlightMovement;
 UPROPERTY(BlueprintReadOnly, Category="Helicopter") TObjectPtr<APawn> PreviousPilotPawn;
 UPROPERTY(BlueprintReadOnly, Category="Helicopter") TObjectPtr<AController> PilotController;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Helicopter") FVector ExitOffset = FVector(-250.f, 180.f, -80.f);
 UFUNCTION(BlueprintCallable, Category="Helicopter") bool EnterHelicopter(APawn* Passenger);
 UFUNCTION(BlueprintCallable, Category="Helicopter") void ExitHelicopter();
 UFUNCTION(BlueprintImplementableEvent, Category="Helicopter") void OnPilotEntered(APawn* PreviousPawn);
 UFUNCTION(BlueprintImplementableEvent, Category="Helicopter") void OnPilotExited(APawn* PreviousPawn);
 virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
 void MoveForward(float Value);
 void MoveRight(float Value);
 void MoveVertical(float Value);
 void TurnYaw(float Value);
 void TiltPitch(float Value);
 UFUNCTION(Server, Reliable) void ServerExitHelicopter();
 void ExitHelicopterAuthority();
};