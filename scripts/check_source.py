"""Dependency-free source contract checks. Not a substitute for an Unreal compile."""
from pathlib import Path
root = Path(__file__).resolve().parents[1]
required = {
    "Build/Android/ShadowProtocolStorage_UPL.xml": ['name="android:installLocation"', 'value="preferExternal"'],
    "Source/ShadowProtocol/ShadowProtocol.Build.cs": ['AdditionalPropertiesForReceipt.Add("AndroidPlugin"', '"ShadowProtocolStorage_UPL.xml"'],
    "Source/ShadowProtocol/SPPlayableLoopAutomation.cpp": ['ShadowProtocol.PlayMode.Validation', 'MissionMachineTypes.Contains', 'DefaultPawnClass'],
    "ShadowProtocol.uproject": ["ShadowProtocol", "ChaosVehiclesPlugin"],
    "Source/ShadowProtocol/ShadowProtocol.Build.cs": ['"AIModule"', '"NavigationSystem"', '"ChaosVehicles"', '"UMG"'],
    "Source/ShadowProtocol/SPGameMode.cpp": ["ASPCombatCharacter::StaticClass()"],
    "Source/ShadowProtocol/SPCombatCharacter.cpp": ["ServerMeleeAttack_Implementation", "ServerInteractWithWorld_Implementation", "ServerFireArrow_Implementation", "HasAuthority()", "Mine == Theirs", "ApplyDamage", "MulticastAttackPerformed"],
    "Source/ShadowProtocol/SPHealthComponent.cpp": ["DOREPLIFETIME", "HasAuthority()", "OnTakeAnyDamage", "RestoreHealth"],
    "Source/ShadowProtocol/SPCombatAIController.cpp": ["TActorIterator", "GetCombatTeam()", "RequestMeleeAttack"],
    "Source/ShadowProtocol/SPMissionObjective.cpp": ["HasAuthority()", "AdvanceScene", "DOREPLIFETIME", "ESPObjectiveRequirement::ActivateMachine", "ESPObjectiveRequirement::ClearEnemies", "ESPObjectiveRequirement::EscapeInVehicle", "CheckOccupants", "GetOverlappingActors"],
    "Source/ShadowProtocol/SPMissionBootstrap.cpp": ["HasAuthority()", "bSpawned", "BotTeam", "EnvironmentClass", "WorldMachineClass", "SpawnActor<ASPStreetBlock>", "SpawnActor<ASPDriveableVehicle>", "SpawnActor<ASPDriveableHelicopter>", "SpawnActor<ASPMissionObjective>", "ESPScene::Debrief", "MissionConsole", "ASPWorldMachine::StaticClass()", "SpawnActor<ASPMissionObjective>(StageType, Origin.TransformPosition"],
    "Source/ShadowProtocol/SPStreetBlock.cpp": ["RebuildBlock", "AddInstance"],
    "Source/ShadowProtocol/SPDriveableVehicle.cpp": ["CanEnterVehicle", "EnterVehicle", "ExitVehicleAuthority", "SetThrottleInput", "SetSteeringInput", "ServerSetDriveInput", "SetVehicleBrake"],
    "Source/ShadowProtocol/SPDriveableHelicopter.cpp": ["EnterHelicopter", "ExitHelicopterAuthority", "HelicopterVertical", "BodyMesh", "ServerMoveFlight", "ServerMobileMove"],
    "Source/ShadowProtocol/SPWorldMachine.cpp": ["CanInteract", "OnMachineUsed", "OnMachineStateChanged", "ArrowCache", "MedicalStation", "ESPWorldMachineType::Trailer", "TurretFireInterval", "DOREPLIFETIME(ASPWorldMachine, MachineType)", "Tick"],
    "Source/ShadowProtocol/SPArrowProjectile.cpp": ["LaunchInDirection", "ApplyDamage", "OnArrowOverlap", "ArrowAsset"],
    "Source/ShadowProtocol/SPEnvironmentZone.cpp": ["BuildEnvironment", "ESPEnvironmentBiome::Forest", "ESPEnvironmentBiome::Suburb", "FallbackConeMesh", "DOREPLIFETIME(ASPEnvironmentZone, Biome)", "AddInstance"],
    "Source/ShadowProtocol/SPMobileControls.cpp": ["SetMoveStick", "SetLookStick", "PressMelee", "PressBlock", "PressSprint", "PressDodge", "PressInteract", "PressArrowFire", "PressExitVehicle", "SetMobileDriveInput", "PressBrake"],
    "Source/ShadowProtocol/SPCharacter.cpp": ["StartSprint", "StopSprint", "ToggleCrouch", "Dodge", "ServerSetSprinting", "DOREPLIFETIME"],
    "Source/ShadowProtocol/SPCombatCharacter.cpp": ["SweepMultiByChannel", "AttackConeDot", "SPMeleeLOS", "ServerSetBlocking"],
    "Source/ShadowProtocol/SPMissionRewards.cpp": ["ESPScene::Debrief", "CompleteMission", "NM_Standalone"],
    "Source/ShadowProtocol/SPProgressionSubsystem.cpp": ["PurchaseCar", "EquipCar", "RewardedMissions", "STREET_COMPACT", "SPORT_COUPE"],
    "Source/ShadowProtocol/SPCoinPickup.cpp": ["CollectPickup", "NM_Standalone"],
    "Source/ShadowProtocol/SPProfileSubsystem.cpp": ["SaveProgress", "RecordLegacyMissionProgress", "SetLocalAlias"],
    "Source/ShadowProtocol/SPProgressionWidget.cpp": ["OnProgressionUpdated", "BuyCar", "SelectCar"],
    "Source/ShadowProtocol/SPGarageSpawner.cpp": ["GetEquippedCar", "OwnsCar", "SpawnActor<ASPDriveableVehicle>"],
    "Source/ShadowProtocol/SPThrillerPresentation.cpp": ["THE BLACK CONVOY", "THE ENFORCER", "LAST EXIT", "PresentBeat"],
    "Config/DefaultInput.ini": ['ActionName="Melee"', 'ActionName="Interact"', 'ActionName="ArrowFire"', 'ActionName="ExitVehicle"', 'ActionName="Brake"', 'ActionName="Handbrake"', 'AxisName="VehicleThrottle"', 'AxisName="VehicleSteering"', 'AxisName="MoveForward"'],
}
errors = []
for relative, markers in required.items():
    path = root / relative
    if not path.is_file():
        errors.append(f"Missing: {relative}")
        continue
    content = path.read_text(encoding="utf-8")
    errors.extend(f"{relative}: missing {marker}" for marker in markers if marker not in content)
if errors:
    raise SystemExit("\n".join(errors))
print(f"Source contract checks passed ({len(required)} files). Unreal compile and runtime remain unverified.")
