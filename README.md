# SHADOW PROTOCOL: WORLD OF ASSASSINS

Unreal Engine 5.5 C++ third-person action-thriller prototype. **Pre-alpha: source checks are not an Unreal compilation or a playable release.**

## Android APK status — NOT BUILT

**There is currently NO APK to download, NO successful Android packaging run, and NO verified installable game.** Do not mistake a workflow source file or source-sanity check for a build artifact. The packaging YAML currently exists only on the `feature/sprint-001-foundation` branch; GitHub may not offer its manual Run workflow button until the workflow file is also present on the default branch. A Windows self-hosted runner with UE5.5 and Android SDK/NDK must be connected first.

- [Inspect Android packaging workflow source (not an APK)](https://github.com/criss-lab/Game-/blob/feature/sprint-001-foundation/.github/workflows/android-apk.yml)
- [Inspect Windows compilation workflow source (not a compiled build)](https://github.com/criss-lab/Game-/blob/feature/sprint-001-foundation/.github/workflows/unreal-build.yml)

After the workflow is made available on the default branch and the runner is connected: Actions → Android APK (UE5.5 self-hosted) → Run workflow → select `feature/sprint-001-foundation`. The runner needs labels `self-hosted`, `Windows`, `X64`, `unreal-5-5`, `android-sdk`, a working UE5.5 Android toolchain, `UE_ROOT`, and `ANDROID_HOME`. After a successful run, download **ShadowProtocol-Android-Development-APK** under Artifacts. This is a development build, not a signed production release; validate installation and gameplay on real devices before distribution. Artifacts expire after 14 days.

## Current foundation

- Third-person character and combat, bots, world interactions, mission stages and offline progression
- Graybox environment generation and helicopter fallback
- Android Low/Mid/High device profiles and Android storage installation preference
- Source sanity checks and opt-in Unreal Windows compile/play-mode workflow

## Release gates

1. UE5.5 Unreal Header Tool and Windows Editor compilation pass
2. Full mission completes in a real playable level, including debrief and save/load
3. Authored visual, sound, animation and touch UI assets are committed and tested
4. Android packaging produces an APK, verified by SHA-256 and installed on representative devices
5. Frame-time, memory, thermal, crash and input tests pass

Do not commit Intermediate, Saved, DerivedDataCache, Binaries, credentials, keystores, or unlicensed assets. Do not claim a downloadable APK until a successful packaging run provides one.
