# Gadget for Tanmatsu

Gadget 0.6.0, SD revision 14, gives Tanmatsu six switchable workspaces for a clock, internet radio, animated eyes, weather forecasts, a demo, and system status. Install it on the SD card. Radio keeps playing while you use another workspace.

## Controls

Press `Space` when the startup guide is ready. Hold `Hyper` while pressing the colored symbol for a workspace, or tap `Hyper` and press the symbol within two seconds. Press `Esc` twice within 850 ms to return to the launcher.

Tap the power button to turn off the display and keyboard backlights while background services continue. Tap it again to resume. **System** shows battery, CPU, heap and Wi-Fi status with signal bars beside it. CPU, heap and Wi-Fi refresh every five seconds. Press `Tab` for the network name, signal strength in dBm, assigned IP addresses and Wi-Fi MAC.

## Internet Radio

Hold `Hyper` and press the orange triangle in the default layout, then use the arrows and `Enter` to play a station. Open Options with the red cross to add a direct MP3 or AAC stream or choose a theme. Use the orange triangle to find stations by name or country. Search looks ahead for playable streams, shows the codec and bitrate when reported, and marks HLS entries **Playback unavailable**.

Press the purple diamond on a search result or `Tab` in Now Playing to see the URL, codec and available bitrate. A rate prefixed with `~` is estimated from decoded audio. Press the cyan clover to save a highlighted preview or Backspace on the player to remove the highlighted station. Station changes are saved on the SD card.

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
