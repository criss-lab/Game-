#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPIdentityComponent.generated.h"
UENUM(BlueprintType)
enum class ESPCharacterLook : uint8 { Operative, Civilian, Police, Thief };
UCLASS(ClassGroup=(ShadowProtocol), meta=(BlueprintSpawnableComponent))
class SHADOWPROTOCOL_API USPIdentityComponent : public UActorComponent
{
 GENERATED_BODY()
public:
 USPIdentityComponent();
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance") ESPCharacterLook CharacterLook = ESPCharacterLook::Operative;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance") TObjectPtr<USkeletalMesh> BodyMesh;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance") TObjectPtr<UAnimInstance> AnimationPreview;
 UFUNCTION(BlueprintCallable, Category="Identity") static FString GenerateRandomAlias();
 UFUNCTION(BlueprintPure, Category="Identity") static bool IsValidAlias(const FString& Alias);
};
