#include "SPDriveableHelicopter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "CollisionShape.h"
ASPDriveableHelicopter::ASPDriveableHelicopter()
{
 PrimaryActorTick.bCanEverTick = true;
 bReplicates = true; SetReplicateMovement(true); bUseControllerRotationYaw = true; bUseControllerRotationPitch = true;
 HelicopterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelicopterMesh")); SetRootComponent(HelicopterMesh);
 static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
 if (BodyMesh.Succeeded()) HelicopterMesh->SetStaticMesh(BodyMesh.Object);
 HelicopterMesh->SetRelativeScale3D(FVector(2.2f, 1.2f, 0.7f));
 HelicopterMesh->SetCollisionProfileName(TEXT("Pawn"));
 CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); CameraBoom->SetupAttachment(HelicopterMesh); CameraBoom->TargetArmLength = 650.f; CameraBoom->bUsePawnControlRotation = true;
 PilotCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PilotCamera")); PilotCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
 FlightMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FlightMovement")); FlightMovement->UpdatedComponent = HelicopterMesh; FlightMovement->MaxSpeed = 1800.f;
}
void ASPDriveableHelicopter::SetupPlayerInputComponent(UInputComponent* Input)
{
 Super::SetupPlayerInputComponent(Input); if (!Input) return;
 Input->BindAxis(TEXT("MoveForward"),this,&ASPDriveableHelicopter::MoveForward);
 Input->BindAxis(TEXT("MoveRight"),this,&ASPDriveableHelicopter::MoveRight);
 Input->BindAxis(TEXT("HelicopterVertical"),this,&ASPDriveableHelicopter::MoveVertical);
 Input->BindAxis(TEXT("Turn"),this,&ASPDriveableHelicopter::TurnYaw);
 Input->BindAxis(TEXT("LookUp"),this,&ASPDriveableHelicopter::TiltPitch);
 Input->BindAction(TEXT("ExitVehicle"),IE_Pressed,this,&ASPDriveableHelicopter::ExitHelicopter);
}
bool ASPDriveableHelicopter::EnterHelicopter(APawn* Passenger)
{
 if (!HasAuthority() || !IsValid(Passenger) || Passenger==this || !Passenger->GetController() || GetController()) return false;
 PreviousPilotPawn=Passenger; PilotController=Passenger->GetController(); PilotController->Possess(this); OnPilotEntered(PreviousPilotPawn); return GetController()==PilotController;
}
void ASPDriveableHelicopter::ExitHelicopter() { if (HasAuthority()) ExitHelicopterAuthority(); else ServerExitHelicopter(); }
void ASPDriveableHelicopter::ServerExitHelicopter_Implementation() { ExitHelicopterAuthority(); }
void ASPDriveableHelicopter::ExitHelicopterAuthority()
{
 if (!HasAuthority() || !IsValid(PreviousPilotPawn) || !IsValid(PilotController) || PilotController->GetPawn()!=this) return;
 APawn* OldPawn=PreviousPilotPawn; AController* OldController=PilotController;
 const FVector DesiredExit = GetActorLocation() + GetActorRotation().RotateVector(ExitOffset);
 if (!GetWorld()) return;
 FCollisionQueryParams Params(SCENE_QUERY_STAT(SPHelicopterExit), false, this);
 Params.AddIgnoredActor(OldPawn);
 const bool bBlocked = GetWorld()->OverlapBlockingTestByChannel(DesiredExit, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeCapsule(42.f, 90.f), Params);
 if (bBlocked) return;
 if (!OldPawn->SetActorLocation(DesiredExit, true)) return;
 OldController->Possess(OldPawn);
 OnPilotExited(OldPawn); PreviousPilotPawn=nullptr; PilotController=nullptr;
}
void ASPDriveableHelicopter::MoveForward(float V) { if (Controller && !FMath::IsNearlyZero(V)) { AddMovementInput(GetActorForwardVector(),V); if (!HasAuthority()) ServerMoveFlight(GetActorForwardVector(),V); } }
void ASPDriveableHelicopter::MoveRight(float V) { if (Controller && !FMath::IsNearlyZero(V)) { AddMovementInput(GetActorRightVector(),V); if (!HasAuthority()) ServerMoveFlight(GetActorRightVector(),V); } }
void ASPDriveableHelicopter::MoveVertical(float V) { if (Controller && !FMath::IsNearlyZero(V)) { AddMovementInput(FVector::UpVector,V); if (!HasAuthority()) ServerMoveFlight(FVector::UpVector,V); } }
void ASPDriveableHelicopter::TurnYaw(float V) { if (Controller) AddControllerYawInput(V); }
void ASPDriveableHelicopter::TiltPitch(float V) { if (Controller) AddControllerPitchInput(V); }

void ASPDriveableHelicopter::MobileMove(FVector2D Axis)
{
 Axis = Axis.GetClampedToMaxSize(1.f);
 if (!Controller) return;
 AddMovementInput(GetActorForwardVector(), Axis.Y);
 AddMovementInput(GetActorRightVector(), Axis.X);
 if (!HasAuthority()) ServerMobileMove(Axis);
}
void ASPDriveableHelicopter::MobileLook(FVector2D Axis)
{
 Axis = Axis.GetClampedToMaxSize(1.f);
 TurnYaw(Axis.X);
 TiltPitch(Axis.Y);
}
void ASPDriveableHelicopter::MobileVertical(float Value)
{
 MobileVerticalInput = FMath::Clamp(Value, -1.f, 1.f);
}
void ASPDriveableHelicopter::Tick(float DeltaSeconds)
{
 Super::Tick(DeltaSeconds);
 if (Controller && !FMath::IsNearlyZero(MobileVerticalInput))
 {
  AddMovementInput(FVector::UpVector, MobileVerticalInput);
  if (!HasAuthority()) ServerMoveFlight(FVector::UpVector, MobileVerticalInput);
 }
}

void ASPDriveableHelicopter::ServerMoveFlight_Implementation(FVector Direction, float Scale)
{
 if (Direction.ContainsNaN() || !FMath::IsFinite(Scale)) return;
 AddMovementInput(Direction.GetSafeNormal(), FMath::Clamp(Scale, -1.f, 1.f));
}
void ASPDriveableHelicopter::ServerMobileMove_Implementation(FVector2D Axis)
{
 if (Axis.ContainsNaN()) return;
 Axis = Axis.GetClampedToMaxSize(1.f);
 AddMovementInput(GetActorForwardVector(), Axis.Y);
 AddMovementInput(GetActorRightVector(), Axis.X);
}
