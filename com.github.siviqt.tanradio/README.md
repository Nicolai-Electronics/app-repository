# TanRadio

TanRadio is an Internet radio player for Tanmatsu. It plays direct MP3 streams
and searches the Radio Browser directory by station name or country.

Configure Wi-Fi in the launcher and insert an SD card. Press Space on the
welcome screen to start. TanRadio begins with an empty station list; press F2
to find stations, then F5 to save a result. Existing station catalogs are
preserved. Saved stations are stored at `/sd/tanradio/stations.txt`.

## Controls

| Key | Action |
| --- | --- |
| Up / Down | Select station or search result |
| Enter | Play selected station; submit a search in the search dialog |
| F1, release, F1 again | Confirm exit to the launcher |
| Esc / F2 at exit prompt | Cancel exit |
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
BSP/LCD/ES8156 versions, build time and build ID. Esc or F2 also closes it.

Playback supports 16-bit mono/stereo MP3 at 44.1 or 48 kHz. Speaker and headphone
volume settings are remembered independently. The app uses BSP 1.6.2 with
TanRadio's tested audio, display and input fixes, LCD abstraction 1.1.0 and
ES8156 0.1.0, built with ESP-IDF 6.0.2.

The matching version and AppFS revision are recorded in `metadata.json`.
The included `TanRadio.bin` is an AppFS application image. Install it through
the Tanmatsu application repository or BadgeLink.
