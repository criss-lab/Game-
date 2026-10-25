using UnrealBuildTool;
public class ShadowProtocol : ModuleRules
{
 public ShadowProtocol(ReadOnlyTargetRules Target) : base(Target)
 {
  PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
  if (Target.Platform == UnrealTargetPlatform.Android)
  {
   AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(ModuleDirectory, "../../Build/Android/ShadowProtocolStorage_UPL.xml"));
  }
  PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "ChaosVehicles", "UMG", "LevelSequence", "MovieScene" });
 }
}
