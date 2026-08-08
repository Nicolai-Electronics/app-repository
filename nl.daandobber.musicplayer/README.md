# MusicPlayer

Audio-reactive MP3/WAV player for the Nicolai Electronics Tanmatsu (ESP32-P4).

Application ID: `nl.daandobber.musicplayer`

The built-in **MilkDrip** visualizer is a compact, low-resolution homage to
MilkDrop. It combines PCM-driven feedback, procedural rendering, continuously
evolving global colour palettes and fluid field-morph transitions designed for
the ESP32-P4.

## Features

- Recursively finds MP3, WAV, FLAC, and OGG files on the microSD card.
- MP3, 8/16/24/32-bit mono/stereo WAV, FLAC, and OGG playback through the ES8156 codec.
- Artist/album library with ID3v1/ID3v2, WAV `LIST/INFO`, FLAC and Ogg Vorbis comment metadata, plus filename/folder fallbacks.
- Editable play queue: replace it with an artist or album, append selections, and remove tracks.
- Hardware volume control and speaker/headphone handling through the Tanmatsu BSP.
- 12-band FFT, RMS, peak and beat analysis of the PCM that is sent to I2S.
- 128 audio-reactive effects: 64 MilkDrop-style feedback presets plus 54 independent procedural, minimal, hybrid and demoscene-inspired renderers.
- Eighteen global colour palettes continuously blend independently of the active algorithm.
- Slow fluid transitions advect the previous image through a vector field while the next algorithm emerges.
- Instant preset switching without an expensive full-frame crossfade.
- Persistent settings for timed/beat-based automatic effect switching, shuffle, visual intensity, brightness and idle dimming.
- Real JPEG album art from MP3 ID3 `APIC`, FLAC `PICTURE` blocks, or nearby `cover.jpg`, `folder.jpg`, or `front.jpg`; generated art is the fallback.
- Scrobble playback to Last.fm automatically when Last.fm is configured and enabled in settings.

## Controls

| Screen | Key | Action |
| --- | --- | --- |
| Library | Left / Right or Tab | Select artists, albums, or playlist pane |
| Library | Up / Down | Select an item in the active pane |
| Library | Page Up / Page Down | Move eight items |
| Artists / albums | Return | Replace playlist and start playing |
| Artists / albums | Space | Append selection to playlist |
| Playlist | Return / Space | Play selected track |
| Playlist | Backspace | Remove selected track |
| Player | Left / Right | Previous / next playlist track |
| Player | Return / Space | Play / pause |
| Player | F2 / F3 or Down / Up | Previous / next effect |
| Player | F4 or Menu | Hide / show the compact now-playing card |
| Any screen | F5 | Open / close settings |
| Settings | Up / Down, Left / Right | Select and change a setting |
| Settings | `Test dimming`, Right | Immediately test the configured dim level |
| Settings | `Last.fm account`, Right | Open the Last.fm account screen |
| Last.fm account | Up / Down, Return | Select a field, Return to edit/login, Escape to cancel/back |
| Player | Escape | Return to library while audio keeps playing |
| Any | Side `+` / `-` | Volume |
| Any | F1 | Stop audio and return to launcher |

## Last.fm scrobbling

Last.fm needs an API key and shared secret from
https://www.last.fm/api/account/create. Open settings with `F5`, scroll to
`Last.fm account`, fill in the API key, shared secret, username and password
(Return toggles editing a field, Return on `Login + save` submits), then set
`LastFM scrobbling` to `On`. MusicPlayer stores the API credentials and the
Last.fm session key in NVS; the password is only used for that login request
and is never saved.

Scrobbling requires WiFi. There is no WiFi setup screen in MusicPlayer itself
- it connects using whatever network is already configured on the device (for
example through another Tanmatsu app that saves WiFi credentials, such as
Disc-O-Matsu, or the badge's own WiFi settings). WiFi is only brought up for
the duration of each Last.fm request and disconnected afterward.

## Build environment

The project targets ESP-IDF 5.5.1 and ESP32-P4. It uses managed components for
`badge-bsp`, PAX, `esp_audio_codec`, `esp-dsp`, and `tanmatsu-wifi` (for Last.fm
scrobbling). SDK and toolchain paths are kept outside this repository.

The plasma and metaballs algorithms are adapted from the MIT-0 licensed
`esp_effects` project by Mika Tuupola. The rendering and all audio-reactive
control code in this application are Tanmatsu/PAX-specific implementations.

`Mind Is Growing` is an original cellular-automaton effect inspired by the
techniques Linus Akesson documented for his 256-byte C64 demo
[A Mind Is Born](https://linusakesson.net/scene/a-mind-is-born/). No original
demo code is included. Copper bars, Kefrens bars, rotozoom, shadebobs and the
twister are original implementations of classic demoscene effect families.
