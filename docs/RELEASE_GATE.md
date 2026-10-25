# Shadow Protocol — evidence-based release gate

Status: PRE-ALPHA. Source-sanity checks only verify repository text markers. The Unreal automation smoke test exists in SPPlayableLoopAutomation.cpp but must run on a UE5.5-equipped Windows self-hosted runner.

| Gate | Pass evidence required |
| --- | --- |
| UE5.5 Win64 editor compile | Windows Unreal workflow logs: UHT, C++ build and ShadowProtocol.PlayMode.Validation pass |
| Actual playable map | Committed cooked .umap, PlayerStart, SPGameMode, placed SPMissionBootstrap, navigation, lighting, collision, safe spawn positions |
| PC playable loop | Standalone recording/log: reach objective, activate console, defeat enemies, enter car/heli, escape, debrief, coins/XP awarded exactly once and persisted |
| Vehicle production setup | Chaos wheel Blueprint and physics asset; helicopter collision, grounded exit, possession/camera; gamepad and keyboard controls |
| Content and polish | Authored models/materials, animations, audio, HUD, objective prompts, interaction feedback, settings, accessibility |
| Android cook/package | Signed arm64 APK or AAB, cooked startup map, merged installLocation checked, install/relaunch and crash log |
| Android real device | Touch HUD actually authored and added to viewport; control and possession transitions; low/mid-tier FPS, memory, heat and load-time measurements; SD/no-SD behavior |
| Regression | Two-player PIE replication, save migration, duplicate rewards, death/respawn, blocked vehicle exits, no objective softlocks |
| Distribution | Versioning, signing-key management, store compliance, privacy/permissions, update and rollback test |

## Known blockers
- No committed authored .umap or final environment assets verified.
- No Unreal Windows self-hosted runner execution evidence; green source-sanity does not imply compile.
- Android touch-handler C++ exists, but a visible authored UMG widget and device testing are still required.
- Chaos vehicle requires proper skeletal mesh/wheel/physics Blueprint setup.
- Online rewards are intentionally disabled until authenticated server-side validation is implemented.
- Gameplay objective triggers depend on collision and map layout; verify actual console and escape volume reachability in editor.

Do not advertise the game as complete or production-ready until every release gate has independent evidence.
