# WadaMesh

A full **[MeshCore](https://github.com/meshcore-dev/MeshCore)** client for the Tanmatsu.
It runs the LoRa mesh, Wi-Fi and Bluetooth radios **all at the same time**, so the device
works as a complete standalone communicator *and* as a phone companion at once — no
mode-switching, no rebooting to swap radios.

MeshCore is an encrypted, infrastructure-free LoRa mesh network — messages hop from device
to device with no cell tower, Wi-Fi or internet in between. WadaMesh gives it a full
graphical front-end on the Tanmatsu's own screen and keyboard.

## Why WadaMesh

- **Multi-radio, simultaneously.** LoRa, Wi-Fi and Bluetooth all live at once. Chat on the
  mesh while Wi-Fi streams map tiles and a phone stays paired over Bluetooth — concurrently.
- **Standalone *and* companion at the same time.** A complete client on its own screen and
  keyboard, while a phone app can connect over Bluetooth or Wi-Fi in parallel. You don't
  have to pick a role.
- **Live mesh map.** Plot yourself and your contacts on a real OpenStreetMap from their
  shared GPS positions (tiles fetched and cached over Wi-Fi).
- **Built for the keyboard.** Arrow-key navigation, type-to-edit and function-key shortcuts —
  designed around the Tanmatsu's hardware, not bolted on from a touchscreen.
- **Polished and localised.** Full-colour emoji and 11 interface languages.

## Features

- **Messaging** — direct messages and channels with unread markers, a "new messages"
  divider and quick replies.
- **Contacts & map** — contact list with live signal and distance, plus the mesh map.
- **Radio control** — frequency / spreading factor / bandwidth / TX power, and GPS.
- **Connectivity** — LoRa, Wi-Fi and Bluetooth at once; use it standalone or as a Wi-Fi +
  Bluetooth companion for a phone.

## Links

- Source & releases: <https://github.com/ALLFATHER-BV/wadamesh>
- Web flasher & project info: <https://wadamesh.com>

## Licence

WadaMesh is distributed under **GPL-3.0** — see [LICENSE](LICENSE). It incorporates
[MeshCore](https://github.com/meshcore-dev/MeshCore) (MIT, © Scott Powell /
rippleradios.com); MeshCore-derived files keep their MIT notices, and the combined
work is GPL (MIT is GPL-compatible). See [NOTICE](NOTICE) for third-party licenses.
