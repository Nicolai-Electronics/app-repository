# Gadget for Tanmatsu

Gadget 0.6.0 gives Tanmatsu six switchable workspaces for a clock, internet radio, animated eyes, weather forecasts, a demo, and system status. Install it on the SD card. Radio keeps playing while you use another workspace.

## Controls

Press `Space` when the startup guide is ready. Use `Hyper` + `F1`–`F6` to switch workspaces and press `Esc` twice within 850 ms to return to the launcher.

Tap the power button to turn off the display and keyboard backlight while background services continue. Tap it again to resume. Open **System** to see the battery percentage and charging state.

## Internet Radio

Press `Hyper` + `F2` in the default layout, then use the arrows and `Enter` to play a station. Press `F1` to add a direct MP3 stream, `F2` to find stations by name or country, `F5` to save a highlighted preview, or `F6` to remove the highlighted station. Changes to the station list are saved on the SD card.

## ELF modules

Gadget loads modules from the SD card at startup. Each module folder contains a `module.json` manifest and a `module.elf` file with its compiled code. A module can provide a workspace, a background service, or both.

## Build a module

Activate the ESP-IDF 6.0.2 toolchain for ESP32-P4 so `riscv32-esp-elf-gcc` is on your `PATH`. From the Gadget source checkout, create a workspace module and build its ELF.

```bash
python3 -B tools/new_module.py face example.face build/example_face --name "Example Face" --slot 5
python3 -B tools/build_module.py -o build/example_face/module.elf build/example_face/module.c
```

To scaffold a background service, use `service` in place of `face`, omit `--slot`, and choose a unique module ID and folder. Copy `module.json`, `module.elf`, and any assets into a new folder under `/sd/apps/<slug>/modules/`. If slot 5 is occupied, assign `example.face` to it using the workspace settings instructions in `docs/DEVELOPMENT.md`. Relaunch Gadget to load the module.

## Source code

Find the Gadget GitHub repository under `repository_url` in the release's `metadata.json`. The checkout has example modules in `examples/`, bundled module source in `modules/`, and the SDK guide in `docs/MODULE_SDK.md`.
