# CHRONOS

CHRONOS is a clock and timekeeping app for the Tanmatsu device. It brings local
and world clocks, alarms, a stopwatch, a countdown timer, and a cadence pacer
together on one screen-driven interface. It also works offline with the device
clock and can optionally synchronize time over Wi-Fi.

## Features

- See local time alongside up to five world cities, including date and daylight
  saving time information.
- Set up to five alarms with one-time, daily, or selected-weekday repeats,
  custom labels, snooze, and ring duration.
- Use a stopwatch with ten recent splits, a configurable countdown, or a pacer
  that gives an audible beat at a chosen interval.
- Check timers and alarms together on the read-only Activity screen.
- Choose a visual theme, text size, clock format, and keyboard, display, and LED
  brightness.
- Use an optional Nokia 5110/PCD8544 or NE-HCS12SS59T-R1 VFD external display.

## Getting started

Start CHRONOS from the Tanmatsu launcher. If CHRONOS is not installed, copy
the complete `chronos` application folder from its installation package to
`/sd/apps/chronos/`, then select it in the launcher. Keep the app binary,
launcher metadata, and icons together.

The first run starts with UTC local time, 24-hour format, five example world
cities, a five-minute countdown, and a one-second pacer. Wi-Fi is off by default;
turn it on in **Settings → Time** if you want to use network time.

## Settings and saving

With an SD card available, CHRONOS stores settings in
`/sd/chronos/settings.ini` and saves committed changes automatically. Choose
**Save to SD** or press **F4** on the Settings screen to write them immediately.
Without an SD card, you can use CHRONOS for the session, but settings will not
be saved after exit. Reinserted cards do not reload settings during the current
session.

Side volume buttons change volume in 5% steps. Volume is saved with settings;
0% mutes playback. Connecting headphones routes sound away from the speaker.

### Alarm sounds and external displays

Choose **Alarm sound** in Settings to select the built-in tone or a WAV file in
`/sd/chronos/`. Custom files must be uncompressed, signed 16-bit PCM WAV, mono
or stereo, sampled at 8,000–48,000 Hz, with no more than 2 MiB of audio data.
Missing or unsupported files fall back to the built-in tone. The file plays
while an alarm is ringing.

Optional external displays are configured under **Settings → External Display**.
They show selected clock or timer information and do not have separate controls;
use the CHRONOS screen and keyboard. Wiring and device-specific setup are in the
`external-display.md`.

## Tips and useful information

- The battery icon in the header is an estimate. A question mark means battery
  information is unavailable.
- LED A indicates alarm state and LED B indicates timer activity. LED brightness
  is a board-wide setting; F6 controls only the display and keyboard backlights.
- **Activity** is a summary screen. To control a timer, open its tab.
- Apply commits a change to the running app. Saving writes committed settings
  to the SD card.

## Troubleshooting

**Settings do not remain after exiting.** Confirm that an SD card is mounted,
then use **Save to SD** or check that automatic saving has completed before
removing the card.

**Sync now is unavailable.** In **Settings → Time**, turn Wi-Fi on, select NTP
as the time source, and enable NTP. CHRONOS uses the Wi-Fi networks configured
in the Tanmatsu launcher.

**A custom alarm sound does not play.** Check that the file is in
`/sd/chronos/`, that its filename is selected in Alarm sound, and that it uses a
supported WAV format. The built-in tone is used if the file is missing or
unsupported.

**The clock time needs correcting.** Check the selected time source and local
timezone. RTC mode uses the Tanmatsu hardware clock; Manual mode needs a fresh
date and time after each restart.

## Known limitations

- Alarms run only while CHRONOS is open and cannot wake a powered-off Tanmatsu.
- Manual time is not retained across application restarts.
- World timezone rules come from the Tanmatsu SDK's timezone catalogue. Unusual
  historical or calendar-based daylight saving changes may not be represented.
- The Nokia 5110/PCD8544 interface is write-only, so CHRONOS cannot confirm
  whether that display is connected.
