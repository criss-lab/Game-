"""Dependency-free source contract checks. Not a substitute for an Unreal compile."""
from pathlib import Path
root = Path(__file__).resolve().parents[1]
required = {
    "ShadowProtocol.uproject": ["ShadowProtocol", "ChaosVehiclesPlugin"],
    "Source/ShadowProtocol/ShadowProtocol.Build.cs": ['"AIModule"', '"NavigationSystem"', '"ChaosVehicles"', '"UMG"'],
    "Source/ShadowProtocol/SPGameMode.cpp": ["ASPCombatCharacter::StaticClass()"],
    "Source/ShadowProtocol/SPCombatCharacter.cpp": ["ServerMeleeAttack_Implementation", "HasAuthority()", "bSameAssignedTeam", "ApplyDamage", "MulticastAttackPerformed"],
    "Source/ShadowProtocol/SPHealthComponent.cpp": ["DOREPLIFETIME", "HasAuthority()", "OnTakeAnyDamage"],
    "Source/ShadowProtocol/SPCombatAIController.cpp": ["TActorIterator", "GetCombatTeam()", "RequestMeleeAttack"],
    "Source/ShadowProtocol/SPMissionObjective.cpp": ["HasAuthority()", "AdvanceScene", "DOREPLIFETIME"],
    "Source/ShadowProtocol/SPMissionBootstrap.cpp": ["HasAuthority()", "bSpawned", "BotTeam", "SpawnActor<ASPStreetBlock>", "SpawnActor<ASPDriveableVehicle>", "SpawnActor<ASPMissionObjective>"],
    "Source/ShadowProtocol/SPStreetBlock.cpp": ["RebuildBlock", "AddInstance"],
    "Source/ShadowProtocol/SPDriveableVehicle.cpp": ["CanEnterVehicle", "HasAuthority()"],
    "Source/ShadowProtocol/SPMobileControls.cpp": ["SetMoveStick", "SetLookStick", "PressMelee"],
    "Source/ShadowProtocol/SPMissionRewards.cpp": ["ESPScene::Debrief", "CompleteMission", "NM_Standalone"],
    "Source/ShadowProtocol/SPProgressionSubsystem.cpp": ["PurchaseCar", "EquipCar", "RewardedMissions", "STREET_COMPACT", "SPORT_COUPE"],
    "Source/ShadowProtocol/SPCoinPickup.cpp": ["CollectPickup", "NM_Standalone"],
    "Source/ShadowProtocol/SPProfileSubsystem.cpp": ["SaveProgress", "RecordLegacyMissionProgress", "SetLocalAlias"],
    "Config/DefaultInput.ini": ['ActionName="Melee"', 'AxisName="MoveForward"'],
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
