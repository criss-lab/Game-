#include "SPDriveableVehicle.h"
#include "Components/InputComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
ASPDriveableVehicle::ASPDriveableVehicle()
{
 bReplicates = true;
 SetReplicateMovement(true);
 VehicleDisplayName = FText::FromString(TEXT("Getaway Vehicle"));
}
bool ASPDriveableVehicle::CanEnterVehicle(APawn* Passenger) const
{
 return IsValid(Passenger) && Passenger != this && Passenger->GetController() != nullptr && GetController() == nullptr;
}
bool ASPDriveableVehicle::EnterVehicle(APawn* Passenger)
{
 if (!HasAuthority() || !CanEnterVehicle(Passenger)) return false;
 PreviousDriverPawn = Passenger;
 PreviousDriverController = Passenger->GetController();
 PreviousDriverController->Possess(this);
 OnVehicleOccupied(PreviousDriverPawn);
 ForceNetUpdate();
 return GetController() == PreviousDriverController;
}
void ASPDriveableVehicle::SetupPlayerInputComponent(UInputComponent* Input)
{
 Super::SetupPlayerInputComponent(Input);
 if (!Input) return;
 Input->BindAxis(TEXT("VehicleThrottle"), this, &ASPDriveableVehicle::InputThrottle);
 Input->BindAxis(TEXT("VehicleSteering"), this, &ASPDriveableVehicle::InputSteering);
 Input->BindAction(TEXT("Brake"), IE_Pressed, this, &ASPDriveableVehicle::InputBrakePressed);
 Input->BindAction(TEXT("Brake"), IE_Released, this, &ASPDriveableVehicle::InputBrakeReleased);
 Input->BindAction(TEXT("Handbrake"), IE_Pressed, this, &ASPDriveableVehicle::InputHandbrakePressed);
 Input->BindAction(TEXT("Handbrake"), IE_Released, this, &ASPDriveableVehicle::InputHandbrakeReleased);
 Input->BindAction(TEXT("ExitVehicle"), IE_Pressed, this, &ASPDriveableVehicle::ExitVehicle);
}
void ASPDriveableVehicle::InputThrottle(float Value) { ThrottleValue = FMath::Clamp(Value, -1.f, 1.f); UpdateDriveInput(); }
void ASPDriveableVehicle::InputSteering(float Value) { SteeringValue = FMath::Clamp(Value, -1.f, 1.f); UpdateDriveInput(); }
void ASPDriveableVehicle::SetMobileDriveInput(FVector2D Axis)
{
 Axis = Axis.GetClampedToMaxSize(1.f);
 ThrottleValue = Axis.Y;
 SteeringValue = Axis.X;
 UpdateDriveInput();
}
void ASPDriveableVehicle::UpdateDriveInput()
{
 if (UChaosWheeledVehicleMovementComponent* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
 {
  Movement->SetThrottleInput(ThrottleValue);
  Movement->SetSteeringInput(SteeringValue);
 }
 if (!HasAuthority()) ServerSetDriveInput(ThrottleValue, SteeringValue);
}
void ASPDriveableVehicle::ServerSetDriveInput_Implementation(float Throttle, float Steering)
{
 ThrottleValue = FMath::Clamp(Throttle, -1.f, 1.f);
 SteeringValue = FMath::Clamp(Steering, -1.f, 1.f);
 if (UChaosWheeledVehicleMovementComponent* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
 {
  Movement->SetThrottleInput(ThrottleValue);
  Movement->SetSteeringInput(SteeringValue);
 }
}
void ASPDriveableVehicle::SetVehicleBrake(bool bPressed)
{
 bBrakePressed = bPressed;
 if (UChaosWheeledVehicleMovementComponent* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent())) Movement->SetBrakeInput(bBrakePressed ? 1.f : 0.f);
 if (!HasAuthority()) ServerSetVehicleBrakes(bBrakePressed, bHandbrakePressed);
}
void ASPDriveableVehicle::SetVehicleHandbrake(bool bPressed)
{
 bHandbrakePressed = bPressed;
 if (UChaosWheeledVehicleMovementComponent* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent())) Movement->SetHandbrakeInput(bHandbrakePressed);
 if (!HasAuthority()) ServerSetVehicleBrakes(bBrakePressed, bHandbrakePressed);
}
void ASPDriveableVehicle::ServerSetVehicleBrakes_Implementation(bool bBrake, bool bHandbrake)
{
 bBrakePressed = bBrake;
 bHandbrakePressed = bHandbrake;
 if (UChaosWheeledVehicleMovementComponent* Movement = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
 {
  Movement->SetBrakeInput(bBrakePressed ? 1.f : 0.f);
  Movement->SetHandbrakeInput(bHandbrakePressed);
 }
}
void ASPDriveableVehicle::InputBrakePressed() { SetVehicleBrake(true); }
void ASPDriveableVehicle::InputBrakeReleased() { SetVehicleBrake(false); }
void ASPDriveableVehicle::InputHandbrakePressed() { SetVehicleHandbrake(true); }
void ASPDriveableVehicle::InputHandbrakeReleased() { SetVehicleHandbrake(false); }
void ASPDriveableVehicle::ExitVehicle()
{
 if (HasAuthority()) ExitVehicleAuthority();
 else ServerExitVehicle();
}
void ASPDriveableVehicle::ServerExitVehicle_Implementation() { ExitVehicleAuthority(); }
void ASPDriveableVehicle::ExitVehicleAuthority()
{
 if (!HasAuthority() || !IsValid(PreviousDriverPawn) || !IsValid(PreviousDriverController) || PreviousDriverController->GetPawn() != this) return;
 APawn* Passenger = PreviousDriverPawn;
 AController* Controller = PreviousDriverController;
 const FVector ExitLocation = GetActorLocation() + GetActorRotation().RotateVector(ExitOffset);
 FHitResult Hit;
 const bool bBlocked = GetWorld() && GetWorld()->SweepSingleByChannel(Hit, ExitLocation, ExitLocation + FVector(0.f,0.f,1.f), FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(45.f));
 if (bBlocked) return;
 Controller->Possess(Passenger);
 Passenger->SetActorLocation(ExitLocation, false, nullptr, ETeleportType::TeleportPhysics);
 OnVehicleVacated(Passenger);
 PreviousDriverPawn = nullptr;
 PreviousDriverController = nullptr;
 ForceNetUpdate();
}
