# Android support — implementation and verification

Status: mobile renderer configuration and Android touch interface foundation added. No Android SDK/NDK, Unreal Android toolchain, physical device run, APK or AAB has been verified.

## Input
A touch interface supplies two virtual joysticks (movement and camera). Action buttons for jump, melee, interact and enter/exit vehicle need a UMG mobile HUD connected to gameplay functions; joystick input alone is not a complete touch UI. Gamepad support uses the existing axis mappings. UI must respect cutouts, safe zones and variable aspect ratios.

## Rendering
Android targets a mobile rendering path, not PC Lumen/Nanite assumptions. Device profiles Android_Low/Mid/High provide starting scalability settings, but selecting them by measured device capability requires a benchmark and DeviceProfileSelector rules. Optimize skeletal meshes, draw calls, textures, shaders, reflections, city streaming, crowd count and vehicle physics. Target 30 FPS baseline on supported midrange devices and 60 FPS only where profiling demonstrates stability. Real device thermals and memory determine supported minimum specs.

## Build
Install Epic-supported UE 5.5 Android SDK/NDK/JDK versions via official documentation. Set up Android platform in Unreal, validate package name, icon, signing, privacy declarations and Play requirements. Cook Android assets, compile, package development APK, install with adb, test offline airplane-mode launch, touch controls, AI and mission objective. Test online only after authentication/session backend exists. Never commit signing keys or passwords.

## Acceptance
Compile Android target; verify touch input, multiple screen sizes, device rotation policy, graphics fallback, loading, audio, battery/thermal behavior, app resume, network disconnect, local saves and crash logs. Separate Android and Windows CI build gates.
