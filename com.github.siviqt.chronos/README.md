# CHRONOS

Current release: **1.2.0 revision 6**. Hardware checks passed on 2026-11-02.

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

1. Start CHRONOS from the Tanmatsu launcher. If CHRONOS is not installed, copy
   the complete `com.github.siviqt.chronos` application folder from its package to
   `/sd/apps/com.github.siviqt.chronos/`, then select it in the launcher. Keep the app binary,
   launcher metadata, and icons together.
2. At the startup screen, press **Space** to open CHRONOS.
3. Use **Left/Right** to highlight a tab and **Enter** to open it. The control
   dock along the bottom of the screen shows the actions available at each step.

The first run starts with UTC local time, 24-hour format, five example world
cities, a five-minute countdown, and a one-second pacer. Wi-Fi is off by default;
turn it on in **Settings → Time** if you want to use network time.

## Using CHRONOS

### Navigation and controls

| Control | Action |
| --- | --- |
| Left / Right | Move between tabs or adjust a value when the dock offers that action. |
| Up / Down | Move through a screen's choices or scroll a list. |
| Enter | Open a screen, edit or choose an item, toggle a setting, or confirm the displayed action. |
| Escape | Cancel the current edit or return to the previous screen. |
| F1 | Exit to the Tanmatsu launcher. If a form has unsaved edits, choose whether to apply, discard, or stay. |
| F2 | Pause or resume the current timer. |
| F3 | Reset the current timer. |
| F4 | Start or stop a timer, apply a settings form, or save settings to the SD card, as shown in the dock. |
| F5 | Open the next tab. Changed forms ask whether to apply, discard, or stay. |
| F6 | Turn both backlights on or off. Timers and other app activity continue. |
| Tab | Record a stopwatch split while the stopwatch is running. |
| Backspace | Open or close the full notifications view when you are not entering text. |
| Side volume buttons | Raise or lower alarm and timer sound volume. |

In text fields, **Left/Right** moves the cursor and **Backspace** deletes text.
Enter accepts the field into the form; it does not apply the whole form. Use
**F4** or the on-screen Apply action to commit a form. Escape cancels the current
field or form, one level at a time. A changed form is never silently discarded
when switching tabs or exiting.

### Clock and world cities

The Clock screen shows local time and up to five world cities. In the default
setup it shows London, New York, Tokyo, Sydney, and Sofia. Highlight a city to
see its time difference from local time. Choose **Sync now** to request an NTP
time update when NTP and Wi-Fi are enabled.

In **Settings → Time**, choose a local timezone, set the 12- or 24-hour format,
and configure the world city list. The timezone and city selectors include a
searchable list; matching ignores letter case and treats spaces and underscores
as equivalent. Selecting a timezone changes how time is displayed and how local
alarm times are interpreted.

Choose one of three time sources in **Settings → Time**:

- **NTP** synchronizes over Wi-Fi using networks already saved in the Tanmatsu
  launcher. CHRONOS tries at startup, hourly, and when you choose Sync now.
- **RTC** reads the Tanmatsu hardware clock. Use **Use Tanmatsu RTC now** to
  set the running clock from it immediately.
- **Manual** lets you enter local date and time. The value is kept for the
  current session only; after restarting in Manual mode, enter the time again.

Wi-Fi is a separate setting and starts Off. CHRONOS does not turn it on for an
NTP request. When Wi-Fi is unavailable, an established system time continues;
duration timers continue independently of clock corrections.

### Alarms

Open **Alarms**, choose one of five slots, and edit its settings. An alarm can
repeat once at the next occurrence of its local time, every day, or on selected
weekdays. A one-time alarm disables itself after it rings. Set its time, label,
snooze interval (1–60 minutes), and ring duration (5–60 seconds), then choose
**Apply alarm** or press **F4**. A weekday alarm needs at least one selected day.

Alarms run only while CHRONOS is open; they do not wake a powered-off device.
While an alarm is ringing, press any letter to snooze all ringing alarms, or
press **Space** to dismiss them. The display wakes when an alarm starts, even if
the backlights were off. Press **Backspace** outside text entry to review full
alarm and storage notifications.

### Stopwatch, countdown, and pacer

The stopwatch records up to ten recent splits, with segment and total times.
Press **Tab** to record a split while it is running. Up and Down scroll through
the split list.

For the countdown and pacer, open **Configuration** to change the duration or
interval. Type a replacement value or use Left/Right for small adjustments,
then apply the form. The countdown accepts 1 second to 60 minutes. Its optional
completion sound lasts up to 15 seconds; press **Space** to acknowledge it. The
pacer accepts intervals from 1 millisecond to about 49.7 days and plays a short
beat at each interval. New settings take effect the next time you start the
pacer.

F2 pauses or resumes, F3 resets, and F4 starts or stops the selected timer.
Running sessions continue when you change tabs or turn off the backlights.

### Settings and saving

The Settings screen is arranged in two columns. Use Left/Right to switch
columns and Up/Down to move within a column. Settings include clock and time
source, local timezone, world cities, NTP, alarm sound, theme, illumination,
font size, and external display.

Some choices, such as theme and font size, apply as soon as you select them.
Forms such as NTP, alarm sound, illumination, and external display use an
explicit Apply action. The bottom dock shows the action for the selected item.

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
`/sd/chronos/` or the installed AppFS package directory
(`/sd/apps/com.github.siviqt.chronos` or
`/int/apps/com.github.siviqt.chronos`). Custom files must be uncompressed,
signed 16-bit PCM WAV, mono or stereo, sampled at 8,000–48,000 Hz, with no more
than 2 MiB of audio data. If the same filename exists in both locations, the
file in `/sd/chronos/` is used.
Missing or unsupported files fall back to the built-in tone. The file plays
while an alarm is ringing.

Optional external displays are configured under **Settings → External Display**.
They show selected clock or timer information and do not have separate controls;
use the CHRONOS screen and keyboard. Wiring and device-specific setup are in the
[external display guide](external-display.md).

## Tips and useful information

- The battery icon in the header is an estimate. A question mark means battery
  information is unavailable.
- LED A indicates alarm state and LED B indicates timer activity. LED brightness
  is a board-wide setting; F6 controls only the display and keyboard backlights.
- **Activity** is a summary screen. To control a timer, open its tab.
- Apply commits a change to the running app. Saving writes committed settings
  to the SD card.

## Troubleshooting

**CHRONOS does not appear in the launcher.** Confirm that the complete
`chronos` package folder, including its metadata and icons, is under
`/sd/apps/com.github.siviqt.chronos/`.

**Settings do not remain after exiting.** Confirm that an SD card is mounted,
then use **Save to SD** or check that automatic saving has completed before
removing the card.

**Sync now is unavailable.** In **Settings → Time**, turn Wi-Fi on, select NTP
as the time source, and enable NTP. CHRONOS uses the Wi-Fi networks configured
in the Tanmatsu launcher.

**A custom alarm sound does not play.** Check that the file is in
`/sd/chronos/` or the installed AppFS package directory, that its filename is
selected in Alarm sound, and that it uses a supported WAV format. The built-in
tone is used if the file is missing or unsupported.

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

## Additional information

- [External display wiring and setup](external-display.md)
- [Third-party licenses](THIRD_PARTY_LICENSES.txt)
