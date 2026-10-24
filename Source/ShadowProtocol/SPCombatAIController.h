#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "SPCombatAIController.generated.h"
UCLASS()
class SHADOWPROTOCOL_API ASPCombatAIController : public AAIController
{
 GENERATED_BODY()
public:
 ASPCombatAIController();
 virtual void OnPossess(APawn* InPawn) override;
protected:
 UPROPERTY(EditDefaultsOnly, Category="AI") float DetectionRadius = 1200.f;
 UPROPERTY(EditDefaultsOnly, Category="AI") float EngagementDistance = 145.f;
 FTimerHandle ThinkTimer;
 void Think();
};
