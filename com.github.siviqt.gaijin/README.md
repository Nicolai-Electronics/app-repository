# Gaijin

Gaijin runs native 32-bit RISC-V NOMMU Linux on Tanmatsu rev7, launched from
AppFS. This development bundle is **0.5.19 / revision 33**, with
**Linux 6.18.35-gaijin-m5**.

It provides:

- BusyBox utilities, the Hush shell and the vi text editor.
- LCD text and USB Serial/JTAG consoles, plus the built-in keyboard.
- LCD as the primary console and no default early USB wait; USB login remains available.
- An SD-backed read-only system and persistent home directories when a state
  image is provisioned; otherwise, user files use temporary RAM storage.
- `shutdown` to return to Launcher, `poweroff` to switch off and `halt` to stop Linux.

USB console, SD storage, persistence, keyboard and framebuffer/fbcon milestones
are accepted. The user confirmed all revision-33 device checks passed on
8 November 2026, including standalone battery boot and late USB attachment.
Wi-Fi, a package manager and a graphical shell are planned.

Keep the SD card inserted while running and preserve `/gaijin/userdata.ext2`
when updating. [Source, installation and documentation](https://github.com/siviqt/Gaijin).
