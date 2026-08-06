# Third-party notices

MultiMesh itself is licensed under the [MIT License](LICENSE). The compiled
firmware statically links the components below, and this file reproduces the
attribution their licences require. Everything here is permissive — MIT,
Apache-2.0 or BSD-3-Clause. No copyleft code is linked or distributed.

The MeshCore and Meshtastic protocol support in MultiMesh is an independent
implementation written from protocol behaviour and published documentation. No
code is derived from the Meshtastic firmware or from any other GPL-licensed
project.

## MIT

Each of the following is distributed under the MIT License, whose full text
appears in [LICENSE](LICENSE) and which requires that these notices accompany
copies of the software, including binaries.

| Component | Copyright |
|---|---|
| [PAX graphics](https://github.com/robotman2412/pax-graphics) (`robotman2412/pax-gfx`) | Copyright (c) 2021-2026 Julian Scheffers |
| `badgeteam/badge-bsp` | Copyright (c) 2025 Nicolai Electronics |
| `badgeteam/custom-certificates` | Copyright (c) 2025 Nicolai Electronics |
| `nicolaielectronics/es8156` | Copyright (c) 2025 Nicolai Electronics |
| `nicolaielectronics/mipi_dsi_abstraction` | Copyright (c) 2024 Nicolai Electronics |
| `nicolaielectronics/mpr121` | Copyright (c) 2025 Nicolai Electronics |
| `nicolaielectronics/ssd1619` | Copyright (c) 2025 Nicolai Electronics |
| `nicolaielectronics/sx126x` | Copyright (c) 2025 Nicolai Electronics |
| `nicolaielectronics/tanmatsu-lora` | Copyright (c) 2026 Nicolai Electronics |
| `nicolaielectronics/tanmatsu-wifi` | Copyright (c) 2025 Nicolai Electronics |
| `nicolaielectronics/tanmatsu_coprocessor` | Copyright (c) 2024 Nicolai Electronics |
| `nicolaielectronics/tca8418` | Copyright (c) 2026 Nicolai Electronics |
| `nicolaielectronics/wifi-manager` | Copyright (c) 2025 Nicolai Electronics |

### PAX graphics font data

`components/mm_ui/font_mono_fi.c` adds six Finnish glyphs to PAX's 7x9 bitmap
font. They are derived from that font's own letterforms rather than drawn by
hand, so the file is a derivative work of MIT-licensed material and carries
Julian Scheffers' copyright notice in full at the top of the file.

## Apache-2.0

Distributed under the Apache License, Version 2.0. A copy is available at
<https://www.apache.org/licenses/LICENSE-2.0>.

- **ESP-IDF** and its bundled components — Copyright (c) Espressif Systems
  (Shanghai) CO LTD
- `espressif/cmake_utilities`, `espressif/eppp_link`, `espressif/led_strip`,
  `espressif/esp_serial_slave_link`, `espressif/wifi_remote_over_eppp` —
  Copyright (c) Espressif Systems (Shanghai) CO LTD
- `espressif/esp_lcd_*` display and touch drivers — Copyright (c) Espressif
  Systems (Shanghai) CO LTD
- `nicolaielectronics/esp-hosted-tanmatsu`,
  `nicolaielectronics/esp-wifi-remote-tanmatsu` — Copyright (c) Espressif
  Systems (Shanghai) CO LTD and Nicolai Electronics

### Mbed TLS

Mbed TLS, bundled with ESP-IDF, is offered under a dual **Apache-2.0 OR
GPL-2.0-or-later** licence. MultiMesh takes it under **Apache-2.0**. The GPL
option is one the recipient may choose, not an obligation this project inherits,
and nothing in MultiMesh is licensed under it.

## BSD-3-Clause

- `nicolaielectronics/bmi270` — Copyright (c) 2025 Nicolai Electronics;
  Copyright (c) 2023 Bosch Sensortec GmbH. All rights reserved.

## Cryptographic constants

Two published network keys appear in the source, and both are public defaults
rather than secrets:

- MeshCore's well-known public channel key (`components/mm_crypto/meshcore_crypto.c`)
- Meshtastic's default PSK (`components/mm_crypto/meshtastic_crypto.c`)

The self-tests use published test vectors from **RFC 8032** (Ed25519) and
**RFC 7748** (X25519).
