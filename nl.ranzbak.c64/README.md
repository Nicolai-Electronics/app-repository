# Commodore 64 emulator

This application is a functional C64 emulator that run on Tanmatsu.

## Usage

### Accessing the menu 

Press the 'purple diamond' button on the device at any time to open or close the menu.

### Navigation

The following keys are used:

| Key            | Description                 |
| -------------- | --------------------------- |
| Purple diamond | Show or hide menu           |
| Up/Down        | Move up and down the menus. |
| Enter          | Activate a menu item        |
| ESC            | Go back one menu up         |


Since the menu structure is still being developed, I'm going to not document more details at this time.

## Games / Sofware

### Loading prg files

At this time only the loading of **.PRG** files is supported.

- To load .prg files, put them on in a directory named 'c64prg' SD card file system.

- By Opening the menu and select 'Load PRG', a .prg file can be selected and loaded.

- When the C64 screen shows again, type the command 'run' and press enter.

### Joystick emulation

The original commodore 64 had two joystick ports namely '1' and '2'.
Because some games use port one and others two, switching the joystick between ports is needed.

In order to enable the Joystick, the 'keyboard joystick' option in the main menu needs to be set to 'yes'.

Selecting the joystick port is done using the 'F5' or 'blue tri-lobe' key.

Indicators of joystick status and port will be added to the software in the future.

#### Joystick key bindings

| key               | Joystick function                |
| ----------------- | -------------------------------- |
| Arrow up          | UP                               |
| Arrow down        | DOWN                             |
| Arrow left        | LEFT                             |
| Arrow right       | RIGHT                            |
| Left SHIFT        | FIRE button                      |
| F5 / Blue diamond | Switch joystick between port 1/2 |


# Source code

Repository: https://github.com/Nicolai-Electronics/tanmatsu-c64

# Copyright statements & license

Copyright (C) 2026 Nicolai Electronics
Copyright (C) 2025 Ranzbak Badge.Team
Copyright (C) 2024 retroelec retroelec42@gmail.com

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

The Commodore name and logo are registered trademarks of Commodore International Corporation.

All organization names, logos, trademarks and artwork including but not limited to the Nicolai Electronics, Commodore, C= and Tanmatsu branding are all rights reserved for their respective owners and may not be reproduced on derrivative works.
