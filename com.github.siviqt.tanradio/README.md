# TanRadio

TanRadio is an Internet radio player for the Tanmatsu handheld.

## Station file

TanRadio keeps its station list on the inserted SD card at
`/sdcard/tanradio/stations.txt`.

On startup, TanRadio checks for this file. If it does not exist, the app creates
the `/sdcard/tanradio` directory and writes an example station catalog
automatically. An existing station file is never overwritten, so user changes
are preserved across app updates and restarts.

Each station occupies one line using the following format:

```text
Name:::Stream URL:::Country code:::Codec:::Bitrate in kbps
```

For example:

```text
Example Radio:::https://example.com/stream.mp3:::BG:::MP3:::128
```

The bitrate is optional. Blank lines and lines beginning with `#` are ignored.
Country codes, when provided, must contain two uppercase letters. Stream URLs
must begin with `http://` or `https://`.

To customize the catalog, close TanRadio and edit `stations.txt` on the SD card.
Invalid entries are skipped when the app loads the file.

## Controls

- Up/Down: select a station
- Enter: select the highlighted station
- F1: exit to the launcher
- F2: mute or unmute
- F3: stop playback
- F4: play or resume the selected station
- F5: play the next station
- Volume keys: adjust volume
- Power button: toggle the screen and keyboard illumination while playback continues
