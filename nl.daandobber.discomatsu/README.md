# Disc-O-Matsu

USB CD/DVD audio player for the Tanmatsu badge. Plug a USB CD/DVD drive into
the USB-A port, insert an audio CD, and it plays the tracks over the
speaker or headphones. If WiFi is available it also grabs the artist,
album, track names, and cover art from MusicBrainz.

## Features

- Play audio CDs from a USB CD/DVD drive.
- Rip the inserted CD manually with `F3` to WAV files on the SD card under
  `/sd/Music`; press `F3` again while ripping to stop and discard the partial
  track file.
- Share ripped files manually with `F4` over WiFi from a browser, including a
  whole-album `.tar` download from each album folder, album deletion, and cover
  lookup/removal.
- Scrobble playback to Last.fm automatically when Last.fm is configured.

WAV rips use this layout:

```text
/sd/Music/Artist - Album (Year)/
/sd/Music/Artist - Album (Year)/Artist - Album - 01 - Title (Year).wav
```

For multi-disc releases the disc marker is inserted after the album name:

```text
/sd/Music/Artist - Album - CD1 (Year)/
/sd/Music/Artist - Album - CD1 (Year)/Artist - Album - CD1 - 01 - Title (Year).wav
```

Last.fm needs an API key and shared secret from
https://www.last.fm/api/account/create. Open the in-app menu with `Esc`, choose
`Last.fm settings`, fill in the API key, shared secret, username and password,
then choose `Login + save`. Disc-O-Matsu stores the API credentials and Last.fm
session key in NVS; the password is only used for that login request.

As an alternative for first-time setup, create
`/sd/discomatsu/config.json` on the SD card:

```json
{
  "lastfm": {
    "api_key": "your-api-key",
    "api_secret": "your-shared-secret",
    "username": "your-lastfm-username",
    "password": "your-lastfm-password"
  }
}
```

The file is only used when no Last.fm session is stored in NVS yet. The API
credentials and resulting session key are saved to NVS; the password is not.
Keep in mind that the password remains plain text in `config.json` until you
remove the file from the SD card.

The WiFi file browser is an active mode: press `F4`, wait for the URL shown on
screen, then open it from another device on the same network. It serves files
under `/sd/Music` for download, offers a whole-album `.tar` link inside album
folders, and can be stopped with `F4` again.

If Disc ID lookup does not find the inserted CD, open `Esc` -> `Metadata
search`, type an artist/album query, choose a MusicBrainz release, and the app
will apply that release's album and track metadata to the current disc.

## Install

Fresh clone, nothing set up yet:

```powershell
.\setup.ps1 -SetupSdk -SetupBadgeLink
.\install-badgelink.ps1
```
