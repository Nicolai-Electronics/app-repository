# Open issues

| ID | Status | Issue / next check |
|---|---|---|
| W01 | Partial: hardware verification | Check SD-root config loading, autostart before WiFi, actual HTTPS timing, sleep/resume and repeated start/stop on Tanmatsu. User reported successful flashing and basic operation on 2026-09-07; the individual extended checks are not yet recorded. |
| W02 | Open: upstream integration | Stock launcher lacks the weather host extension. Ship/apply the companion patch before using this plugin; propose a general theme-aware widget/HTTP API upstream before a broad release. |
| W03 | Open: crowded headers | Verify light/dark themes, long UTF-8 city names and other status plugins together. Weather shortens text when space is unusually narrow; below 32 pixels it cannot display. |
| W04 | Open: provider availability | wttr.in returned HTTP 500 during the live smoke test. Its parser and fallback are covered by fixtures; Open-Meteo and MET Norway responded successfully. |
| W05 | Open: custom service formats | Custom URLs support the documented normalized JSON schema. Add named adapters if users need other existing API formats or HTML websites. |
| W06 | Open: on-device memory | Measure total heap/TLS peak and task stack high-water mark on hardware. The parser itself does not allocate; response/config storage is transient. |

# Improvement suggestions

- Add a details view with provider name, forecast timestamp, complete location
  name and a manual refresh action.
- Offer Fahrenheit and alternative wind units while keeping normalized internal
  units and the same small config file.
- Optionally retain last-known weather with an explicit age/stale marker during
  an outage. Current behavior shows the requested error message instead.
- Cache IP location between refreshes and invalidate it when the network changes,
  once the launcher exposes a reliable network identity event.
- Add a configurable provider order and explicit adapters for authenticated APIs.

# Resolved during development

- R01: SDK add-on/EEPROM build mismatch resolved by updating the isolated launcher
  from 7043218 to upstream 20022fc (add-on 0.1.0, BSP 1.4.0).
- R02: Fixed-width widget placement and missing theme context handled by the
  companion host extension and measured header boundaries.
- R03: Incomplete/HTTPS-unconfigured SDK HTTP helper avoided through the bounded,
  certificate-verifying weather host helper.
- R04: SDK incremental plugin link dependency corrected in the plugin CMake file.
- R05: Large config buffers moved off the service stack; bounded transient
  allocations are freed after each update.
