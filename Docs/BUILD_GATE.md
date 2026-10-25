# Build gate: source sanity is not engine compilation

The Source sanity workflow has completed successfully on commit 97c01b0c (run 40127370886). This only validates source markers.

A separate manually dispatched Unreal build workflow is supplied. It requires an actual Windows x64 self-hosted GitHub runner labelled unreal-5-5 with Unreal Engine 5.5 and C++ build tools installed, and UE_ROOT configured on that runner. GitHub-hosted runners do not include Unreal Engine or project assets. Do not dispatch unless the runner exists.

After successful editor compile, run the play-mode automation test and manually test a combat graybox in PIE. The project still has no committed map asset or cinematic art. Windows package, Android package, multiplayer, and production quality remain unverified.

Security: never upload Unreal credentials, signing keys, proprietary engine binaries or paid assets into this public repository. Use an authorized engine installation and appropriate runner permissions.
