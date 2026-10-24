#include "SPDriveableVehicle.h"
#include "Components/InputComponent.h"
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
 return HasAuthority() && IsValid(Passenger) && Passenger != this && Passenger->GetController() != nullptr && GetController() == nullptr;
}
bool ASPDriveableVehicle::EnterVehicle(APawn* Passenger)
{
 if (!CanEnterVehicle(Passenger)) return false;
 PreviousDriverPawn = Passenger;
 PreviousDriverController = Passenger->GetController();
 PreviousDriverController->Possess(this);
 OnVehicleOccupied(PreviousDriverPawn);
 ForceNetUpdate();
 return GetController() == PreviousDriverController;
}
void ASPDriveableVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
 Super::SetupPlayerInputComponent(PlayerInputComponent);
 if (PlayerInputComponent) PlayerInputComponent->BindAction(TEXT("ExitVehicle"), IE_Pressed, this, &ASPDriveableVehicle::ExitVehicle);
}
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
