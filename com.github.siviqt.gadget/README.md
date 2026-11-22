# Gadget for Tanmatsu

Gadget gives Tanmatsu six switchable workspaces for a clock, internet radio, animated eyes, weather forecasts, a demo, and system status. Radio keeps playing while you use another workspace.

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
