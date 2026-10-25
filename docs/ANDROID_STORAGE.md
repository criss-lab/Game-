# Android installation and SD-card policy

The Android packaging module registers `Build/Android/ShadowProtocolStorage_UPL.xml`, which requests `android:installLocation="preferExternal"` on the generated manifest. This is a preference, **not** a guarantee that the game installs on a removable SD card. Android decides based on device configuration, OS policy, available space and app compatibility; the user may need to move the app through system settings. Adoptable storage differs from portable SD storage. Do not require SD hardware or request broad storage permissions just to install the app.

The app must remain functional when no SD card exists. Keep essential executable code, saves and user settings in Android-managed app storage; never put save files on an arbitrary removable path. Large optional content should be packaged/delivered in platform-supported asset packs or downloaded to a user-approved location using Android's supported storage APIs, with integrity checks and recovery if media is removed. UE packaging may put some files in internal storage even when the APK is externally installed.

## Physical device acceptance
1. Cook/package Android and inspect the **generated merged AndroidManifest.xml** or APK manifest for `android:installLocation="preferExternal"`; source CI does not verify manifest merging.
2. Install on a device with portable SD, one with adoptable storage, and one without SD. Check actual install location in Android settings; never claim guaranteed SD installation.
3. Launch, play, save, restart, and update. Remove/unmount portable SD safely and check Android's behavior; avoid corruption and provide user-friendly content recovery.
4. Compare load times on slow SD vs internal storage, free-space warnings, frame pacing, memory, and thermal throttling. External installation alone is not a performance optimization.
