#include "Misc/AutomationTest.h"
#if WITH_DEV_AUTOMATION_TESTS
#include "SPMissionBootstrap.h"
#include "SPMissionObjective.h"
#include "SPSceneDirector.h"
#include "SPWorldMachine.h"
#include "SPGameMode.h"
#include "SPCombatCharacter.h"
#include "SPProgressionTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSPPlayableLoopValidation, "ShadowProtocol.PlayMode.Validation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FSPPlayableLoopValidation::RunTest(const FString& Parameters)
{
 const ASPMissionBootstrap* Bootstrap = GetDefault<ASPMissionBootstrap>();
 const ASPMissionObjective* Objective = GetDefault<ASPMissionObjective>();
 const ASPGameMode* GameMode = GetDefault<ASPGameMode>();
 TestNotNull(TEXT("Bootstrap CDO"), Bootstrap);
 TestNotNull(TEXT("Objective CDO"), Objective);
 TestNotNull(TEXT("Game mode CDO"), GameMode);
 if (!Bootstrap || !Objective || !GameMode) return false;
 TestEqual(TEXT("Mission begins at infiltration"), static_cast<int32>(Objective->AdvanceTo), static_cast<int32>(ESPScene::Infiltration));
 TestEqual(TEXT("Default objective requires reaching trigger"), static_cast<int32>(Objective->Requirement), static_cast<int32>(ESPObjectiveRequirement::Reach));
 TestTrue(TEXT("Five-stage mission has enemy encounters"), Bootstrap->EnemyCount > 0);
 TestTrue(TEXT("Mission has security console"), Bootstrap->MissionMachineTypes.Contains(ESPWorldMachineType::SecurityConsole));
 TestTrue(TEXT("Default pawn is combat character"), GameMode->DefaultPawnClass == ASPCombatCharacter::StaticClass());
 TestTrue(TEXT("Scene order ends with debrief"), static_cast<uint8>(ESPScene::Debrief) == static_cast<uint8>(ESPScene::VehicleEscape) + 1);
 AddInfo(TEXT("Source-level Unreal automation smoke test only. Full map, possession, combat, mission rewards and Android device tests are separate acceptance gates."));
 return true;
}
#endif
