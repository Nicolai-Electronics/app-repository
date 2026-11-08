# Source provenance and licenses

- `components/libfdt/`: unmodified libfdt source from Linux 6.18.35
  `scripts/dtc/libfdt` (DTC 1.7.2-g52f07dcc), plus upstream DTC v1.7.2
  `libfdt/fdt_check.c`, omitted by Linux's import. `fdt_overlay.c` is omitted
  because M0 does not apply overlays. Original per-file copyright notices
  remain; choose **BSD-2-Clause** from its dual license. Terms:
  `LICENSES/BSD-2-Clause.txt`. Local CMake integration is original Gaijin code.
- `linux/patches/0001-gaijin-esp32p4.patch`: consolidated current Linux
  platform and drivers, preserving per-file copyright and GPL notices.
  Platform, NOMMU pool, cache and timer work derives from
  [mrbreaker/why2025-linux](https://github.com/mrbreaker/why2025-linux/tree/bd790395c14968a27c00f962c9cd08d66f39de24)
  at `bd790395c14968a27c00f962c9cd08d66f39de24`. Gaijin supplies its own
  Tanmatsu board DTS and configuration; WHY2025 wiring is not included.
  USB Serial/JTAG earlycon derives from
  [tsuyoshi-otake/m5stamp-p4-c6-linux](https://github.com/tsuyoshi-otake/m5stamp-p4-c6-linux/tree/84b6160288763d61f6f9d5975bf73148da5a724f)
  at `84b6160288763d61f6f9d5975bf73148da5a724f` (v0.4.3).
  GPL terms: `LICENSES/GPL-2.0.txt`.
- The consolidated patch's polled USB TTY, GPIO/management-MCU and SD host
  integrations are original Gaijin GPL-2.0-only code. Register facts were
  checked against pinned ESP-IDF v6.0.2 P4 LL/register headers; no ESP-IDF
  implementation is linked into Linux. MCU register definitions and key bit
  order derive from the MIT Nicolai Electronics coprocessor interface pinned
  in `dependencies.lock`; its attribution remains in the driver/keymap and
  `LICENSES/Tanmatsu-coprocessor-MIT.txt`. The original MIT `m2-input-test`
  uses the same supported userspace ABI.
- `main/display_commands.h`: Tanmatsu ST7701 command sequence copied from
  Nicolai Electronics' MIT `mipi_dsi_abstraction` **1.2.1**, pinned by
  `dependencies.lock`. Original copyright and identifier are retained.
  `main/display.c` adapts ESP-IDF v6.0.2's Apache-2.0 DPI setup and uses its
  public HAL and driver APIs; original Espressif attribution is retained.
  The native Linux framebuffer driver is original GPL-2.0-only code using
  pinned register/descriptor facts; no ESP-IDF runtime is linked into Linux.
- M1 userspace: Buildroot **2025.02.15**, pinned in `sources.lock.json`,
  builds BusyBox (GPL-2.0), uClibc-ng (LGPL-2.1-or-later), GCC/binutils,
  libgcc and elf2flt under their respective upstream terms. These are
  fetched dependencies, not vendored binary blobs. Before redistributing a
  built initramfs, retain corresponding sources and notices from Buildroot's
  `legal-info` output, including applicable static-linking/relink materials
  and toolchain runtime notices. The original ABI probe is MIT licensed.
- The native bounded-write SD host uses the pinned ESP-IDF P4 descriptor format
  and transfer driver as its hardware reference, BSP 1.6.4 Tanmatsu wiring,
  Linux streaming DMA APIs and DesignWare register definitions. The SD base
  uses Linux's GPL FAT, loop and SquashFS implementations. M3 adds the
  kernel's ext2 and device-mapper linear support; `gaijin-state-map` is original
  MIT-licensed userspace code linked with the same static uClibc toolchain. Buildroot builds
  host squashfs-tools **4.6.1** (GPL-2.0-or-later) and its compression
  dependencies; these are build tools, not target executables.
- `main/handoff.c`: PMP grant and direct M-mode handoff adapted from
  WHY2025's `linux-native/main/main.c` at the revision above, under its
  **Apache-2.0 OR GPL-2.0-only** identifier. The remaining quiesce sequence
  uses ESP-IDF's APIs. Apache terms: `LICENSES/Apache-2.0.txt`.
- ESP-IDF and registry components are downloaded build dependencies with
  their own license files; their resolved identities are in `dependencies.lock`.
  BSP 1.6.4's MIT storage implementation is used directly, not copied or
  modified. Radio and coprocessor firmware are not redistributed or rebuilt.

`sources.lock.json` records source URLs, versions and revisions. Original
project specification documents are user-supplied source material and retain
their existing ownership.

The native power controller uses the AppFS 2.1.0 retained-selection magic/layout
as a protocol reference and ESP-IDF v6.0.2's ROM reset symbol. It preserves the
loader-supplied record rather than linking AppFS or ESP-IDF into Linux.
`gaijin-power` is original MIT userspace code using the existing NOMMU ABI.
