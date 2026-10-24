# World and vehicle production — Mission 001

This branch contains a street-layout actor and a Chaos wheeled vehicle base class. It does not contain photorealistic environment assets, a drivable configured vehicle Blueprint, street map, traffic, pedestrians or packaged game.

## Streets and environment
Use an original Nairobi-inspired fictional district, not a claim of an exact real-world replica. Build road hierarchy, junctions, sidewalks, shops, drainage, street signs, traffic signals, pedestrian crossings, weather, ambient traffic and time-of-day lighting. ASPStreetBlock can instance artist-supplied meshes for a repeatable graybox; assign compatible 100 cm reference meshes to Road, Sidewalks and Buildings, then call RebuildBlock in editor. Its dimensions are placeholders and need asset-specific correction. Add collision, NavMesh, streaming/World Partition, occlusion, lighting and platform LODs. Do not use unlicensed photos, logos or scans.

## Vehicle pipeline
Use Chaos Vehicles: rig skeletal mesh, four wheel bones, wheel classes, physics asset, engine/torque/transmission, tire friction, suspension, steering, input mapping, camera and audio. Derive a Blueprint from SPDriveableVehicle. Begin with fictional sedan, police interceptor, delivery van and getaway SUV, with realistic mass and tuning based on original references. CanEnterVehicle is only an authority-side eligibility helper: seat transition, possession, replicated occupant state, exit safety and controls remain to be built. Never treat this as a working drivable car.

## Photorealism targets
PC: Nanite geometry where suitable, Lumen, PBR scanned-style materials from licensed sources, decals, puddles, weather and volumetric effects; authored facial/body animations. Mobile: baked/optimized lighting where needed, simplified materials, mesh LODs, constrained traffic and memory budgets. Capture performance on real hardware. Art direction must maintain recognizable gameplay silhouettes and accessible visual contrast.

## Acceptance
Unreal 5.5 editor compile, graybox road placement, configured Chaos vehicle test, physics and collisions, scene streaming, NPC pathfinding, day/night, PC/mobile performance captures and multiplayer vehicle authority tests.
