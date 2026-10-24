using UnrealBuildTool;
public class ShadowProtocol : ModuleRules
{
 public ShadowProtocol(ReadOnlyTargetRules Target) : base(Target)
 {
  PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
  PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "ChaosVehicles", "UMG", "LevelSequence", "MovieScene" });
 }
}
