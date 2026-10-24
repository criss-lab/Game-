# Mission 001: The Black Convoy — graybox assembly

This is an editor assembly guide, NOT a shipped playable level. No .umap, art assets, packaged executable or verified compilation exists yet.

1. In Unreal Engine 5.5 create /Game/Maps/L_BlackConvoy_Graybox and configure World Settings GameMode Override = SPGameMode.
2. Add PlayerStart, floor/walls, lighting and a NavMeshBoundsVolume covering the combat area; press P to inspect navigable surfaces.
3. Place SPSceneDirector in level. Set up six mission stages: Briefing, Infiltration, Ambush, BossFight, VehicleEscape, Debrief.
4. Place SPCombatCharacter as an enemy with Auto Possess AI = Placed in World or Spawned and AI Controller Class = SPCombatAIController. Give it a collision capsule and a skeletal mesh/animation Blueprint before visual testing.
5. Place SPMissionObjective actors with Director set to the scene director and AdvanceTo set sequentially. Objective overlaps advance scene on server, once per trigger. In a final mission replace generic overlaps with explicit mission rules, cinematic gates and save checkpoints.
6. PIE: WASD to move, mouse to look, Space to jump, left mouse to melee. Use gamepad mappings where supported.
7. Verify hit distance, health replication, cooldown, AI navigation, objective sequence and both standalone and 2-client PIE.
8. Test dedicated server and packaged Windows and Android builds. Do not label feature complete before these checks pass.

Limitations: No art/cinematics, animations, death flow, UI, bot spawning, win conditions, friends service, session transport or cross-platform device tests yet. AI currently targets player index 0, suitable only for a one-player prototype, not multiplayer production. Damage has no friendly-fire/team filtering yet: do not expose to public matches.
