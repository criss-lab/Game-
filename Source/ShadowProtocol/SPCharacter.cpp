#include "SPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
ASPCharacter::ASPCharacter() { bUseControllerRotationYaw=false; GetCharacterMovement()->bOrientRotationToMovement=true; USpringArmComponent* Boom=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); Boom->SetupAttachment(RootComponent); Boom->TargetArmLength=350.f; Boom->bUsePawnControlRotation=true; UCameraComponent* Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); Camera->SetupAttachment(Boom,USpringArmComponent::SocketName); }
