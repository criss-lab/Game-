# Multiplayer and scene production specification

Status: foundational code, not a functioning networked game.

## Modes
- Solo vs AI: local player against server-controlled AI (AI controllers and behavior trees pending).
- Private duel: selected consenting friend joins via invite (identity, authentication, friend list and session transport pending).
- Cops and Thieves: two teams, initially four per side, server-authoritative team assignment. Thieves steal an objective and reach extraction; police intercept before timer expires. Objective, timer, scoring, spawning and win-state implementation pending.

## Networking
Use a dedicated Unreal server as authority for damage, movement validation, mission objectives, teams and scoring. Never trust a client-supplied team, kill or reward. Use replicated PlayerState for team metadata and GameState for match clock and score. Lobby selection is a *request*, validated on the server; no arbitrary selection of strangers without their acceptance. Friend list must be opt-in, privacy-preserving, block/report capable, and invite-based. Sessions, authentication, reconnect and matchmaking require a chosen online subsystem/backend.

## Scenes
Briefing -> Infiltration -> Ambush -> BossFight -> VehicleEscape -> Debrief. Scene director only accepts sequential authority-side transitions. Blueprint scene hooks can trigger Level Sequences and mission-specific events. Actual level assets, animation, cutscenes and checkpoints are not yet created.

## Acceptance tests before calling this playable
- Build and package server, PC client and Android client.
- Two clients see identical replicated team assignments.
- Team cap and invalid team requests are rejected.
- Disconnect/reconnect and host migration policy tested (dedicated server recommended).
- Damage and scoring remain authoritative under latency and packet loss.
- Solo AI and lobby invites are implemented and exercised end to end.
- Cross-play compatibility, device performance, accessibility and abuse controls validated.
