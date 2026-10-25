#pragma once
#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "SPDriveableVehicle.generated.h"
UCLASS()
class SHADOWPROTOCOL_API ASPDriveableVehicle : public AWheeledVehiclePawn
{
 GENERATED_BODY()
public:
 ASPDriveableVehicle();
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vehicle") FText VehicleDisplayName;
 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Vehicle") float MaxVisualDamage = 100.f;
 UPROPERTY(BlueprintReadOnly, Category="Vehicle") TObjectPtr<APawn> PreviousDriverPawn;
 UPROPERTY(BlueprintReadOnly, Category="Vehicle") TObjectPtr<AController> PreviousDriverController;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle") FVector ExitOffset = FVector(0.f, 220.f, 80.f);
 UFUNCTION(BlueprintImplementableEvent, Category="Vehicle") void OnVehicleOccupied(APawn* PreviousPawn);
 UFUNCTION(BlueprintImplementableEvent, Category="Vehicle") void OnVehicleVacated(APawn* PreviousPawn);
 UFUNCTION(BlueprintCallable, Category="Vehicle") bool CanEnterVehicle(APawn* Passenger) const;
 UFUNCTION(BlueprintCallable, Category="Vehicle") bool EnterVehicle(APawn* Passenger);
 UFUNCTION(BlueprintCallable, Category="Vehicle") void ExitVehicle();
 UFUNCTION(BlueprintCallable, Category="Vehicle") void SetMobileDriveInput(FVector2D Axis);
 UFUNCTION(BlueprintCallable, Category="Vehicle") void SetVehicleBrake(bool bPressed);
 UFUNCTION(BlueprintCallable, Category="Vehicle") void SetVehicleHandbrake(bool bPressed);
 virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
private:
 UFUNCTION(Server, Reliable) void ServerExitVehicle();
 UFUNCTION(Server, Unreliable) void ServerSetDriveInput(float Throttle, float Steering);
 UFUNCTION(Server, Reliable) void ServerSetVehicleBrakes(bool bBrake, bool bHandbrake);
 void ExitVehicleAuthority();
 void UpdateDriveInput();
 void InputThrottle(float Value);
 void InputSteering(float Value);
 void InputBrakePressed();
 void InputBrakeReleased();
 void InputHandbrakePressed();
 void InputHandbrakeReleased();
 float ThrottleValue = 0.f;
 float SteeringValue = 0.f;
 bool bBrakePressed = false;
 bool bHandbrakePressed = false;
};