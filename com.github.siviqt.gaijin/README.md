# Gaijin

Gaijin runs native 32-bit RISC-V NOMMU Linux on Tanmatsu rev7, launched from
AppFS. This development bundle is **0.5.18 / revision 32**, with
**Linux 6.18.35-gaijin-m5**.

It provides:

- BusyBox utilities, the Hush shell and the vi text editor.
- LCD text and USB Serial/JTAG consoles, plus the built-in keyboard.
- An SD-backed read-only system and persistent home directories when a state
  image is provisioned; otherwise, user files use temporary RAM storage.
- `shutdown` to return to Launcher, `poweroff` to switch off and `halt` to stop Linux.

USB console, SD storage, persistence and keyboard milestones are accepted.
LCD framebuffer/fbcon support is implemented; M5 hardware acceptance, remaining
display flicker and current-build device validation are still open. Wi-Fi,
a package manager and a graphical shell are planned.

Keep the SD card inserted while running and preserve `/gaijin/userdata.ext2`
when updating. [Source, installation and documentation](https://github.com/siviqt/Gaijin).
