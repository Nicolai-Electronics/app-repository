# Session handoff — 2026-09-07

Weather 0.1.0 is implemented, cross-built, and includes the plugin binary,
configuration example, icons, tests, companion launcher source and launcher.patch.
The user reported that flashing and testing worked on their Tanmatsu.
See VALIDATION.md for software checks and ISSUES.md for outstanding work.

## Next direction discussed

Prepare reusable launcher capabilities for upstream integration before distributing
Weather in the official repository. No upstream submission is authorized yet.

- Replace Weather-specific drawing with a documented theme-aware widget interface
  exposing available space and an explicit screen identity, rather than matching
  the literal Home title.
- Improve the shared HTTP API for verified HTTPS, complete bounded reads and
  cancellation, instead of a Weather-specific helper.
- Define a named cooperative-stop flag and review shutdown/app-launch paths so a
  stalled network request cannot indefinitely block normal use.
- Replace runtime function-table extension with the normal SDK export mechanism.
- Version the resulting API and enforce firmware compatibility. Weather currently
  declares API 3.0.0 but requires two functions absent from stock firmware.
- Add regression coverage for existing status plugins, service shutdown and app
  launching; record detailed on-device test results.

The current binary requires the companion patch. Generalize/merge launcher support,
release compatible firmware, then rebuild and submit the plugin package.

This directory is the Weather source project. The launcher SDK is at
/home/siv/Development/tanmatsu-sdk/tanmatsu-launcher. Do not confuse the package
repositories at /home/siv/Development/app-repository and
/home/siv/Development/tanmatsu-sdk/app-repository. This checkpoint concerns the
Weather source project only. No push or new device flash is requested.
