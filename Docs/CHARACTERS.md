# Character identity and photorealism pipeline

Status: identity foundation and asset hooks only. No licensed MetaHuman, skeletal mesh, animation, photogrammetry, facial capture or cinematic character asset has been imported into this repository.

## Character creator
Offer Generate Random Name and Edit Name. Random aliases are local suggestions such as Ghost-0421. In online sessions, the server must validate uniqueness, moderation, reserved words, Unicode policy and identity ownership before accepting an alias; never treat a display name as authentication. Do not publish real-world player names without consent.

## Photorealistic character pipeline
Create original characters with properly licensed high-detail body/head assets, realistic skin and eye shaders, groom hair, animation retargeting, facial rig, motion capture and LODs. Use Unreal MetaHuman where its license and target-platform terms permit. Establish PC cinematic LOD and mobile performance LOD separately; use platform profiling before selecting materials and hair rendering.

## Editor integration
Add SPIdentityComponent to player character Blueprint; assign appearance presets and meshes in Blueprint. GenerateRandomAlias and IsValidAlias are exposed to Blueprint for menu UI. The runtime component does not yet apply meshes or persist identity; wire it to a save-game and replicated PlayerState after asset production.
