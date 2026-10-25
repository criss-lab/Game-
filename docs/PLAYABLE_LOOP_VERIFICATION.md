# Playable-loop verification — UE 5.5

Source contract CI is NOT an Unreal compile or runtime test. Use a Windows machine with UE 5.5, Chaos Vehicles and Android SDK/NDK installed.

## Map setup
1. Open ShadowProtocol.uproject, compile the ShadowProtocol editor target and fix all UHT/C++ errors before packaging.
2. Create a persistent playable map and place SPMissionBootstrap. Assign a configured Chaos-wheeled VehicleClass Blueprint (skeletal mesh, physics asset, wheel classes, engine and transmission). Assign an optional configured HelicopterClass. Assign custom EnemyClass and ObjectiveClass if available.
3. Confirm a PlayerStart and SPGameMode with SPCombatCharacter default pawn. Bake navigation around the mission area; provide collision and safe vehicle spawn/exit space. Ground and machines have primitive graybox fallbacks, not final art.
4. Set environment biome and mesh assets. Save the actual map and add it to packaged maps. Verify objective triggers and machine console are reachable. The escape objective is offset 1800 cm from VehiclePosition; adjust to suit the map.
5. Verify the mission console is activated before advancing to Ambush. Clear living non-player combatants within each ClearEnemies objective radius before entering its trigger. A vehicle or helicopter controlled by a player must overlap the final escape trigger. Only standalone play awards local coins/XP; online rewards require authenticated server validation.

## PC runtime acceptance
- Launch PIE standalone; explore with keyboard/gamepad; check sprint, crouch, dodge, melee/block and arrow hit, friendly fire and ammo.
- Press E near console, confirm replicated bActivated and advance. Defeat hostiles, confirm dead actors do not block clearance, advance through BossFight and VehicleEscape.
- Enter a properly configured Chaos car with E, drive, brake, handbrake, exit with F and re-enter; test blocked exit. Alternatively board helicopter and check lift, steering, landing and exit.
- Drive/fly into escape volume. Confirm Debrief and exactly one 500-coin/350-XP mission-1 award, then restart and verify save and duplicate reward prevention.
- Run 2-player PIE listen server/client. Validate machine activation, objective state, arrow damage, vehicle possession and replication; do not treat local reward path as online-ready.

## Android runtime acceptance
- Package arm64 APK with an actual cooked startup map, sign appropriately and install on a physical device.
- Create a UMG widget derived from SPMobileControls and AddToViewport for the local player; this repository exposes touch handlers but does not yet contain an authored visible HUD asset.
- Wire movement/look, melee/block, arrow, interact, vehicle exit, brake/handbrake, and helicopter vertical control. Verify camera and pawn possession updates when entering/exiting machines.
- Profile frame time, heat, memory, input latency, collision, and orientation on a real low/mid-range device. Verify save/relaunch, offline play and no softlock on failed vehicle entry.

## Release blockers
No Unreal compile, cooked .umap, Blueprint vehicle physics setup, actual HUD, signed APK, multiplayer end-to-end test, or real-device run has been verified by source-sanity CI. Do not label PC or Android production-ready until the above gates pass.
