#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPSceneDirector.h"
#include "SPThrillerPresentation.generated.h"
USTRUCT(BlueprintType)
struct FSPSceneBeat
{
 GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite) ESPScene Scene = ESPScene::Briefing;
 UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ChapterTitle;
 UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ObjectiveLine;
 UPROPERTY(EditAnywhere, BlueprintReadWrite) FText RadioLine;
};
UCLASS()
class SHADOWPROTOCOL_API ASPThrillerPresentation : public AActor
{
 GENERATED_BODY()
public:
 ASPThrillerPresentation();
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Presentation") TObjectPtr<ASPSceneDirector> Director;
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Presentation") TArray<FSPSceneBeat> Beats;
 UFUNCTION(BlueprintCallable, Category="Presentation") void BindDirector(ASPSceneDirector* InDirector);
 UFUNCTION(BlueprintImplementableEvent, Category="Presentation") void PresentBeat(const FSPSceneBeat& Beat);
protected:
 virtual void BeginPlay() override;
private:
 void OnScene(ESPScene Scene);
};
