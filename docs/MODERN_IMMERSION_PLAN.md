# Modern immersive ("4D") production plan

4D here means multisensory game feedback, not literal four-dimensional rendering or phone hardware that does not exist.

## PC cinematic target
- Authored night-city lighting, weather variation, wet-surface materials, reflections, restrained bloom, exposure control, cinematic color grading and readable silhouettes.
- Third-person camera shoulder swap, speed-based FOV with motion-sickness toggle, brief optional impact shake, camera collision and smooth vehicle transition.
- Spatialized weapon/footstep/engine ambience, reverb zones and music layers linked to infiltration, ambush, boss fight and escape.
- Responsive HUD: compact objective tracker, interaction prompt, health/ammo, driving/flight controls and reward feedback. No intrusive flashing or forced engagement loops.
- Reusable Niagara effects with particle-count limits; no expensive effect spawned unbounded.

## Android parity and performance
- Preserve existing Android_Low, Android_Mid, Android_High profiles; do not force desktop Lumen/Nanite or expensive volumetrics on phones.
- Material and particle fallbacks, reduced foliage density, dynamic resolution testing, configurable camera shake, battery/thermal checks.
- Touch HUD must be authored and tested; C++ touch handlers alone do not provide visible controls.
- Use headphones for spatial audio where supported; vibration only when device supports it and provide an off switch.

## Acceptance evidence
- Screenshots and video from actual UE5.5 editor/cooked build, not concept art.
- FPS/frame-time, memory, thermal and startup measurements from representative Android hardware.
- PC keyboard/controller and Android touch complete mission including helicopter escape, debrief and save reload.
- No claims of finished visuals until meshes, materials, sound, UMG and map assets are committed and cooked.
