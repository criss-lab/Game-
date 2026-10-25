# SHADOW PROTOCOL: WORLD OF ASSASSINS

Unreal Engine 5.5 C++ third-person action-thriller prototype. **Pre-alpha: source checks are not an Unreal compilation or a playable release.**

## Android APK downloads

**No verified downloadable APK exists yet.** The links below lead to the real build workflow and its runs; an APK download becomes available inside a successful run's **Artifacts** section after a UE5.5 Android-capable self-hosted runner packages the game.

- [Build Android APK / view workflow](https://github.com/criss-lab/Game-/actions/workflows/android-apk.yml)
- [View Android APK build runs](https://github.com/criss-lab/Game-/actions/workflows/android-apk.yml?query=branch%3Afeature%2Fsprint-001-foundation)
- [Windows Unreal compilation workflow](https://github.com/criss-lab/Game-/actions/workflows/unreal-build.yml)

To build: Actions → Android APK (UE5.5 self-hosted) → Run workflow → select `feature/sprint-001-foundation`. The runner needs labels `self-hosted`, `Windows`, `X64`, `unreal-5-5`, `android-sdk`, a working UE5.5 Android toolchain, `UE_ROOT`, and `ANDROID_HOME`. After a successful run, download **ShadowProtocol-Android-Development-APK** under Artifacts. This is a development build, not a signed production release; validate installation and gameplay on real devices before distribution. Artifacts expire after 14 days.

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
