#include "SPDriveableVehicle.h"
#include "GameFramework/Pawn.h"
ASPDriveableVehicle::ASPDriveableVehicle()
{
 bReplicates = true;
 SetReplicateMovement(true);
 VehicleDisplayName = FText::FromString(TEXT("Getaway Vehicle"));
}
bool ASPDriveableVehicle::CanEnterVehicle(APawn* Passenger) const
{
 return HasAuthority() && IsValid(Passenger) && Passenger->GetController() != nullptr && GetController() == nullptr;
}
