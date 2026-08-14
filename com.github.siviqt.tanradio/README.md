# TanRadio

TanRadio is an Internet radio player for the Tanmatsu handheld.

## Station file

TanRadio reads stations from `/sdcard/tanradio/stations.txt` on the inserted
SD card. Each non-empty line contains a station name and stream URL separated
by a pipe character:

```text
Station Name|https://example.com/stream.mp3
```

Create the `/tanradio` directory at the root of the SD card and place the file
there before launching TanRadio.

## Controls

- Up/Down: select a station
- Enter: play the selected station
- F1: exit to the launcher
- F2: mute or unmute
- F3: stop playback
- F4: resume playback
- Volume keys: adjust volume
- Power button: toggle the screen while playback continues
