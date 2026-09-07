# Tanmatsu Weather 0.1.0

A background service plugin displaying weather between Home and the system
icons. **Requires the companion launcher patch**; this binary will not load on
an unpatched launcher because its two weather host functions are absent.
Compiled and tested on the development computer. On 2026-09-07 the user reported
successfully flashing and testing the plugin; extended device acceptance remains pending.

## Behavior

Displays location, current temperature, today's forecast high/low, relative
humidity, cloud cover/overcast status, and wind direction/speed. Units are Celsius
and km/h; direction indicates where the wind comes from. Missing values display
`n/a`, including MET Norway's unavailable full-local-day high/low values. Zero
is a valid temperature, humidity, cloud cover, and wind speed.

The entire summary is shown when it fits. Otherwise four groups rotate every
8 seconds: location/current temperature; high/low; humidity/cloud; wind. The
header's actual font, size and foreground color are used at every draw, including
light/dark and colored themes. A small sun/cloud glyph uses the same foreground.
The weather widget appears on Home only and reserves the title and system icons.

Fetches once on start (including launcher autostart), then every 30 minutes on
the original schedule. It also refreshes when WiFi returns, covering startup
before WiFi is ready. Autostart is initially off; enable it through the launcher's
plugin management UI. Stopping releases the widget, mutex and response storage.
The host patch supports cooperative shutdown so a DNS/TLS operation is not killed
while holding networking resources.

| Situation | Header |
|---|---|
| WiFi unavailable, or no Internet endpoint reachable | `No Internet access` |
| Internet reachable but weather services fail | `Weather sites not available` |
| Location unresolved or responses contain no usable weather | `No weather info found` |
| One metric missing/invalid | `n/a` for that metric |

Connectivity diagnosis is best effort: an HTTPS response from a location/weather
server establishes reachability; otherwise an example.com HTTPS probe is used.
A network that blocks all of these cannot be distinguished from Internet loss.

## Install

1. Update the SDK using [SDK-UPDATE.md](SDK-UPDATE.md), apply `launcher.patch`,
   build the launcher and install that launcher on the device using your usual
   firmware update procedure. The task does not flash the device automatically.
2. Put `weather.plugin` and `plugin.json` in
   `/sd/plugins/com.github.siviqt.weather/` (device path), or the corresponding
   `/int/plugins/com.github.siviqt.weather/` directory.
3. Optionally copy `weather/weather_plugin.txt` to the SD card's root at
   `/weather/weather_plugin.txt` (device mount path `/sd/weather/weather_plugin.txt`).
4. Start Weather in the launcher plugin manager; optionally enable autostart.

The repository package includes an example config in the plugin directory.
It does not overwrite a user's SD-root config. Without that file the plugin uses
public-IP geolocation. A complete manual latitude/longitude pair avoids geocoding;
`location` then supplies the display label. A city name alone uses Open-Meteo
geocoding. IP lookup tries ipwho.is then ipapi.co. VPNs and mobile gateways can
produce a distant location, so manual coordinates are preferable in that case.
Changes to the file take effect on the next update or plugin restart. Invalid
settings fail visibly instead of silently selecting another location.

## Weather providers and custom URLs

Built-in order: Open-Meteo, wttr.in, MET Norway. No keys are embedded or needed.
Each is attempted at most once per update until usable metrics are obtained;
partial results keep missing metrics as `n/a`. The plugin does not mix dates or
fields from different forecasts. Current values from forecast models are estimates,
not readings from a sensor on the Tanmatsu.

Up to four `custom_url=` lines can precede the built-ins. URLs must use HTTPS.
`{lat}` and `{lon}` are replaced with the chosen coordinates. Custom services must
return this JSON shape; omitted/null/invalid fields become `n/a`:

```json
{
  "temperature_c": 22.4,
  "high_c": 26,
  "low_c": 17,
  "humidity_pct": 64,
  "cloud_pct": 95,
  "wind_degrees": 225,
  "wind_kmh": 12
}
```

High/low must describe the selected location's current calendar day. An arbitrary
HTML weather page or an incompatible API JSON response requires an adapter to
this schema. Secrets in configured URLs remain on the SD card and are not logged.
Configuration is limited to 4095 bytes, four URLs of up to 767 bytes, and a
63-byte location. Responses must be smaller than 64 KiB; oversized or truncated
responses fail over. The JSON parser allocates no memory and limits nesting to 24.

## Build and checks

After activating ESP-IDF v6.0.2:

```bash
cmake -S . -B build \
  -DTANMATSU_PLUGIN_SDK=/home/siv/Development/tanmatsu-sdk/tanmatsu-launcher/tools/plugin-sdk
cmake --build build
bash tests/run.sh /home/siv/Development/tanmatsu-sdk/tanmatsu-launcher
```

The launcher must have downloaded its managed components first. The plugin build
adds explicit object dependencies to work around the SDK helper's incremental
relink issue. `launcher/` contains the two new host files; `launcher.patch` also
includes header layout and cooperative stop integration. No managed component
source is patched. See [VALIDATION.md](VALIDATION.md) for measured sizes and tests,
and [ISSUES.md](ISSUES.md) for device checks and improvement suggestions.

## Data attribution

Weather data: [Open-Meteo](https://open-meteo.com/en/docs),
[wttr.in](https://github.com/chubin/wttr.in), and
[MET Norway](https://api.met.no/weatherapi/locationforecast/2.0/documentation).
The Open-Meteo and MET Norway data attribution follows their published CC BY terms;
respect each provider's current usage limits and attribution requirements when
distributing or operating custom endpoints. Geolocation:
[ipwho.is](https://ipwhois.io/documentation) and [ipapi.co](https://ipapi.co/api/).
