# Weather launcher integration backup

This branch preserves the working Weather plugin and its companion launcher
integration. It is a development backup, not an upstream-approved release.
The user reported successful flashing and basic device testing on 2026-09-07.

## Saved changes

- Theme-aware drawing and header measurements place weather between Home and
  the system icons, using the launcher's font and colors.
- A dedicated HTTPS helper verifies server certificates, reads complete bounded
  responses, limits their size and checks for cancellation.
- Cooperative service shutdown lets network operations clean up rather than
  forcibly terminating their task.

Weather source checkpoint: `2dbfa0f`.
Launcher integration commit: `29d9b9fd87e01652cae16db6a2aec14dfaafedb1`.
Launcher base commit: `20022fce316d768a1ff966b973ee2127511713f5` in Nicolai-Electronics/tanmatsu-launcher.

`launcher.patch` is the plain patch; `launcher-integration.commit.patch` preserves
commit metadata for applying with git am. They represent the same integration;
apply only one. The `launcher/` directory also contains the two added host files.
The source, binary, build instructions, tests and configuration example are all
included in this directory.

## Upstream work still needed

Before general distribution, replace the Weather-specific interfaces with reusable
widget/theme and HTTP APIs, define and document safe cooperative shutdown, use the
normal SDK export mechanism, and version/enforce the new API requirements. Release
a compatible launcher before publishing the dependent plugin to standard users.
No submission to the official Nicolai-Electronics repository has been made.
