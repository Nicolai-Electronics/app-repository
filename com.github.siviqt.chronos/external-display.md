# Chronos external displays

External displays are optional secondary information displays. The internal
framebuffer is never copied. Support defaults to **Disabled**, including when
loading older settings. Enable it under **Settings → External Display**.
Use Enter to toggle support, open a choice list, or type a numeric value.
Left/Right adjusts advertised values without opening an editor; booleans use
Enter only. I2C address entry takes hexadecimal digits without `0x`.
Use **Apply External Display** or F4 to
commit the draft. Esc returns from Hardware configuration to its parent; Esc at
the outer form cancels the draft. Changes use Chronos' existing automatic SD
save and explicit Save to SD action. Without an SD card they are session-only.
No application restart is required.

## Selection and status

- **Automatic detection** probes only the configured VFD address, once on CATT
  and then once on the internal bus (or just the selected bus). An address ACK
  is not a unique model ID: connect the documented module at that address.
- **Nokia 5110 / PCD8544** must be selected explicitly. Its interface is
  write-only; initialization success cannot prove attachment or visible pixels.
  Settings therefore says **Nokia initialized - connection unverified**.
- **VFD** uses the NE-HCS12SS59T-R1 protocol described below.
- Configuration, initialization and communication errors remain visible in
  Settings. **Apply and Detect / Retry** is the explicit retry action. There is
  no automatic retry loop after failure. Re-enabling support or changing external
  configuration also allows a new attempt.
- **Disabled** does not allocate a worker, probe, initialize or project content
  at startup. Disabling a running display queues one shutdown; the worker then
  sleeps indefinitely without hardware work.

## Content

**Automatic** selects a ringing alarm, then Countdown, Stopwatch, Pacer, then
Clock. Concurrent running sessions use **Countdown > Stopwatch > Pacer**.
Countdown's existing 15-second completion notification also participates until
it ends or Space acknowledges it. Paused/stopped sessions, enabled but quiet
alarms and snoozed alarms do not override another active source. When an alarm
ends, the controller immediately selects the current eligible source again.

**Fixed content** is independent of internal navigation and stays fixed even
while an alarm rings. Choices use the application's functional tab identities:
Clock, Alarms, Stopwatch, Countdown, Pacer and Activity. **World Time** is also
available because world cities belong to the Clock tab; its separate city
selector is independent of the internal UI's selected city.

Reset Stopwatch shows zero, reset Countdown shows the configured duration, and
idle Pacer shows its configured period. Alarms shows the first ringing slot,
then the first snoozed slot, then the first enabled slot, or All off. Activity
summarizes all three sessions (I idle, R running, P paused, C complete, S stopped).
Unknown stored content IDs remain stored and show Unavailable. An unavailable
world city shows Unavailable (No cities if the city list is empty) and a missing civil clock shows No data.

The Nokia presents four clipped information lines using the reference 5×7 font.
The VFD uses 12 ASCII characters, abbreviating functions as SW, TM, PC and AL.
Clock time uses spaces (`21 19 35`) because the tube renders colons as large bars.
World Time uses a three-character city abbreviation and its time (`Tok 21 19 35`,
or `Tok 09 19 PM` in 12-hour mode). Stopwatch and Countdown show whole seconds
with unit letters (`TM 04M27S  R`); tenths are omitted, not rounded, and remain
available on the internal screen and Nokia display. Timing views use a final
state letter: I ready/idle, R running, P paused, C complete, S stopped. Up to
four minute digits fit without losing seconds or the state (`SW 9999M59SR`). The Nokia
updates at most 5 times/second, VFD at most 10; Clock normally changes once per
second. Both compare their actual rendered output and skip identical transfers.
Unsupported glyphs are simplified to question marks by the display renderer.

## Nokia hardware

| Signal | CATT physical pin | ESP32-P4 GPIO default |
|---|---:|---:|
| RST | 8 | 5 |
| CE | 5 | 15 |
| DC | 7 | 4 |
| DIN | 10 | 3 |
| CLK | 9 | 2 |
| VCC | 1 | 3.3 V |
| GND | 2 | Ground |
| LIGHT | — | Disconnected |

The reference uses **software SPI**, not an ESP-IDF SPI host. Its per-byte CE,
MSB-first data, 2 µs setup/clock delays and reset-low/reset-high waits of at least
20 ms are retained. There is no SPI host, baud-rate or backlight control to set.
The framebuffer is 84×48 pixels / 504 bytes, vertical banks. Initialization is
`21 04 14 (80|contrast) 20 08`, cleared RAM, then normal display `0C`:
temperature coefficient 0, bias 4, default contrast 60 (editable 0–127).

The five signal assignments are editable, restricted to CATT GPIOs 2, 3, 4, 5,
15 and 34. Duplicate assignments are rejected before initialization. GPIOs 12
and 13 are reserved for CATT I²C, and internal display/audio/radio/SD/USB pins
cannot be selected. GPIOs 2–5 overlap **external JTAG**; that interface must not
be connected while using this wiring. No general dynamic pin-ownership registry
exists in this BSP; the supported native application must own these pins.

## VFD hardware

The actual module is **Nicolai Electronics NE-HCS12SS59T-R1**, using a Samsung
HCS-12SS59T 12-character VFD behind its custom I²C controller board. The protocol
is adapted from `external_display_driver/I2C_VFD/main.c` and the supplied module
PDF, not from a generic VFD controller.

- 3.3 V QWIIC/Stemma QT I²C; **100 kHz**.
- Default configured address **0x13**, matching the reference's jumper setting.
  Editable documented module range: 0x10–0x2F. Set physical jumpers to match.
- CATT: I²C host 1, **SDA GPIO 12, SCL GPIO 13**, glitch filter 7, internal pullups.
- Internal: existing BSP host 0, **SDA GPIO 9, SCL GPIO 10**, protected by its
  existing semaphore. Chronos never creates, resets or deletes that bus.
- Address **0x10 is never probed internally** because the reference identifies
  a conflict with onboard hardware. Explicit Internal + 0x10 is invalid.
- Register 0: system control (1 enables, 0 disables). Register 3 is set to zero
  to disable previous scrolling, register 1 to zero to reset offset, register 6
  to configured filament current. Register 10 precedes exactly 12 space-padded
  ASCII characters per update.
- “Brightness / filament current” is limited to **0–110**, default 110, as
  recommended by the supplied implementation/documentation. The register is
  supported by module firmware v2; firmware v1 ignores it.
- Probe deadline 20 ms; transfer deadline 50 ms; shared-bus claim deadline 20 ms.

The pinned Chronos BSP lacks the newer CATT API. The local transport adapts the
newer SDK's `badge_bsp_catt.c` bus settings. It creates CATT only when enabled,
refuses an already-owned host instead of assuming its wiring, and deletes only
its own CATT bus. No managed component or SDK source is modified. Bus, address
and current are editable; fixed module dimensions, clock and board bus pins are
shown as information instead of meaningless configurable fields.
