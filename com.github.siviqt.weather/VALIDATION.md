# Validation — 2026-09-06

- Plugin cross-build: PASS, ESP-IDF v6.0.2 / RISC-V GCC 15.2.0.
- Full patched launcher build: PASS against upstream `20022fce316d768a1ff966b973ee2127511713f5`.
- Patch application check: PASS against local launcher 7043218; reverse check
  confirms the tested upstream copy matches the delivered patch.
- Repository's own metadata validator/schema: PASS.
- Package references and all plugin dynamic imports: PASS against actual host
  symbol tables, including the two weather extension exports.
- Core and service tests: PASS with AddressSanitizer and UndefinedBehaviorSanitizer.
  Cases include malformed JSON, missing/null values, legitimate zeros, invalid
  ranges, UTF-8 escapes, manual coordinates including 0/0, invalid config,
  custom endpoint priority, offline handling, fallback, empty responses,
  periodic refresh and timer wraparound.
- Live Open-Meteo response: PASS, 696 bytes, all seven metrics parsed.
- Live MET Norway response: PASS, 39,134 bytes, five metrics parsed; full-day
  extrema correctly remain missing.
- Live wttr.in response: HTTP 500. Its expected format is fixture-tested and
  first-provider failure is covered in service tests.
- Icon inspected at 64 pixels; package includes 16/32/64 pixel PNGs and SVG source.
- At the time of the 2026-09-06 build, no device flash or hardware test had been performed.
- Update 2026-09-07: the user reported successfully flashing and testing the plugin.
  Detailed hardware acceptance results were not supplied. No remote publication performed.

## Size and memory

Plugin file: **20,688 bytes**. Loaded sections: **13,030 bytes**
(12,310 text/read-only, 568 data, 152 BSS), excluding loader overhead.
SHA-256: `385ced919f0f248a1215bbfe6095d8057814aaa3db887d1159f1812e0a638487`.

Each update temporarily allocates a 64 KiB response buffer and approximately
3.2 KiB config structure, then frees both. The parser allocates nothing. Only
one provider is requested at a time. The host also owns TLS/network allocations
and the launcher allocates an 8 KiB service task stack; total hardware peak still
needs measurement. Idle service checks local WiFi/stop state once per second;
network refresh is every 30 minutes plus reconnect. The header is rendered by
the existing Home redraw cycle, with no new framebuffer or bundled font.

The SDK linker reports its existing RWX segment warning. Plugin linking follows
the SDK's BadgeELF script; basic hardware operation is now user-confirmed; extended acceptance remains open.
