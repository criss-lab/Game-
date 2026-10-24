# Sprint 001 verification

The repository contains source scaffolding only. It has not been compiled or packaged by this integration.

1. Install Unreal Engine 5.5 and a compatible C++ toolchain.
2. Clone repository and generate project files from ShadowProtocol.uproject.
3. Build the ShadowProtocolEditor target, open editor and create a test map with PlayerStart.
4. PIE: verify WASD movement, mouse camera, space jump and controller axes.
5. Confirm game mode spawns ASPCharacter; configure map override if needed.
6. Create a Blueprint subclass of USPOfflineSave and exercise local SaveGameToSlot / LoadGameFromSlot. Do not store online credentials in local saves.
7. Verify offline startup in airplane mode; online mode and bot AI are NOT implemented yet.
8. Package Windows before testing Android; establish device-specific mobile UI and rendering settings.
9. Test two-client replication and server authority before claiming multiplayer works.

The current input wiring uses Unreal legacy axis/action mappings as a bootstrap. Migrate to Enhanced Input mapping contexts when actual input assets and UI are authored. Never claim build success without logs.
