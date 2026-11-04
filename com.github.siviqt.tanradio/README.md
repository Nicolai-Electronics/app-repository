# TanRadio

TanRadio is an Internet radio player for Tanmatsu. It plays direct MP3 streams
and searches the Radio Browser directory by station name or country. Now
Playing shows ICY track information when the stream provides it and scrolls
long labels. Station logos can come from the SD card or an opt-in Radio Browser
lookup.

Configure Wi-Fi in the launcher and insert an SD card. Press Space on the
welcome screen to start. TanRadio begins with an empty station list; press F2
to find stations, then F5 to save a result. Existing station catalogs are
preserved. Saved stations are stored at `/sd/tanradio/stations.txt`.

## Controls

| Key | Action |
| --- | --- |
| Up / Down | Select station or search result |
| Enter | Play selected station; submit a search in the search dialog |
| F1 | Open Options / Exit. Press F1 again to stop playback and return to the launcher |
| Esc / F2 in Options | Cancel and return to the previous screen |
| F3 in Options | Open TanRadio Settings |
| F2 | Open station search; close search or About |
| Tab in search | Switch between station name and country |
| F3 | Stop playback |
| F4 | Play, mute or unmute |
| F5 | Play next saved station; save selected result in search |
| Backspace | Confirm removal of a saved station; edit text in search |
| F6 / Power | Toggle screen and keyboard illumination |
| Volume keys | Adjust the current output's saved volume |
| Hyper (between left Ctrl and Alt) | Open or close About |

About shows the application version/revision, firmware build, ESP-IDF and
BSP/LCD/ES8156 versions, detected display variant and panel ID, build time and
build ID. Esc or F2 also closes it.

## Settings

Open **F1 → F3**. Use **Up/Down** to select a setting and **Left/Right**
to change it. Press **F4 Apply** to save or **F2 / Esc** to discard unapplied
edits.

- **LED brightness** controls the RGB status LEDs from 0% to 100% in
  5 percentage-point steps. The default is 50%.
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

Playback supports 16-bit mono/stereo MP3 at 44.1 or 48 kHz. Speaker and headphone
volume settings are remembered independently. The app uses BSP 2.0.0 with
TanRadio's retained audio, display and input fixes, LCD abstraction 2.0.0 and
ES8156 0.1.0, built with ESP-IDF 6.0.2.

The firmware detects the original or v2 built-in panel at startup and restores
saved VCOM calibration.

## Installation

`metadata.json` records the version and AppFS revision. Install
`TanRadio.bin` through the Tanmatsu application repository or BadgeLink.
