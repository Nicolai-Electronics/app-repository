# TanRadio

TanRadio 0.9.1 (AppFS revision 18) is an Internet radio player for Tanmatsu. It
plays MP3, AAC/AAC+, Ogg Opus and Vorbis, native FLAC, and unencrypted audio-only
HLS over MPEG-TS or packed ADTS/MP3. Now Playing shows ICY track information when
the stream provides it and scrolls long labels. Station logos can come from the
SD card or an opt-in Radio Browser lookup.

Configure Wi-Fi in the launcher and insert an SD card. Press Space on the
welcome screen to start. TanRadio begins with an empty station list; press F2
to find stations, then F5 to save a result. Existing station catalogs are
preserved. Saved stations are stored at `/sd/tanradio/stations.txt`.

## Find stations

Press **F2** to open Radio Browser search, then use **Tab** to switch between
station name and country. Enter a name or a country code such as `BG`, then
press **Enter**. Use **Up/Down** to browse alphabetical results as pages load.
The footer marks **Start of results**, **End of results**, or **Only result**
at a confirmed boundary. **F4** previews and **F5** saves the highlighted
station. Press **Enter** to retry a failed page.

## Controls

The on-screen shortcuts use the Tanmatsu function-key and Backspace symbols.

| Key | Action |
| --- | --- |
| Up / Down | Select station or search result |
| Enter | Play selected station; submit a search in the search dialog |
| F1 · red cross | Open Options / Exit. Press again to stop playback and return to the launcher |
| F2 · orange triangle in Options | Cancel and return to the previous screen |
| F3 · yellow square in Options | Open TanRadio Settings |
| F2 · orange triangle | Open station search or close search and About |
| Tab in search | Switch between station name and country |
| F3 · yellow square | Stop playback |
| F4 · green circle | Play, mute or unmute. Preview the selected search result |
| F5 · cyan three-lobed symbol | Play the next saved station or save the selected result in search |
| Backspace · ⌫ | Open the removal confirmation for a saved station or delete a character in search |
| F6 · purple diamond / Power | Toggle screen and keyboard illumination |
| Volume keys | Adjust the current output's saved volume |
| Hyper (between left Ctrl and Alt) | Open or close About |

About shows the application version/revision, firmware build, ESP-IDF and
BSP/LCD/ES8156 versions, detected display variant and panel ID, build time and
build ID. The orange triangle labels **Close**. Esc and Hyper also close About.

When **⌫ Remove** is visible, press Backspace to open the confirmation. Press
the green circle to remove the station or the orange triangle to cancel.

## Settings

Press the **red cross (F1)** and then the **yellow square (F3)**. Use
**↑ ↓ Select** to choose a setting and **← → Edit** to change it. The
**green circle (F4)** applies settings and the **orange triangle (F2)** goes
back. F2 and Esc discard unapplied edits.

- **LED brightness** controls the RGB status LEDs from 0% to 100% in
  5 percentage-point steps. The default is 50%.
- **Theme** switches the built-in screen between Midnight and Vivid.
  **F4 Apply** saves the selected theme for the next launch.
- **External display** defaults to Off. Select **Nokia 5110** to show
  the station, track, playback state, battery and volume on a PCD8544 LCD.
  Contrast defaults to 60.
- **Download station logos** defaults to Off. It uses Radio Browser for
  stations with a directory UUID. Local logos on the SD card take precedence.

For the Nokia 5110, connect the LCD pins to the Tanmatsu CATT header as shown
below. Use 3.3 V and leave the LCD's `LIGHT` pin disconnected. Unplug the
external JTAG adapter while using the display.

| LCD pin | RST | CE | DC | DIN | CLK | VCC | GND |
| --- | --- | --- | --- | --- | --- | --- | --- |
| CATT physical pin | 8 | 5 | 7 | 10 | 9 | 1 | 2 |

Playback accepts signed 16-bit mono/stereo PCM at 8–48 kHz. It plays 44.1 and
48 kHz at their native rates and resamples other supported rates to 48 kHz.
Speaker and headphone volume settings are remembered independently.
The app uses BSP 2.0.0 with
TanRadio's retained audio, display and input fixes, LCD abstraction 2.0.0 and
ES8156 0.1.0, built with ESP-IDF 6.0.2.

The firmware detects the original or v2 built-in panel at startup and restores
saved VCOM calibration.

## Installation

`metadata.json` records the version and AppFS revision. Install
`TanRadio.bin` through the Tanmatsu application repository or BadgeLink.
