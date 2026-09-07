# Update your local Tanmatsu SDK

Your launcher checkout is `/home/siv/Development/tanmatsu-sdk/tanmatsu-launcher`.
The plugin SDK is its `tools/plugin-sdk` subdirectory. ESP-IDF v6.0.2 is already
installed; the application repository is a separate collection of packages.

Run these commands in a terminal, stopping if an update/build command fails:

```bash
cd /home/siv/Development/tanmatsu-sdk/tanmatsu-launcher
git status --short
```

The checkout was clean when inspected. If this now lists changes, save those
changes before updating. Then:

```bash
git pull --ff-only
git submodule update --init --recursive
source ./env

idf.py -B build/tanmatsu \
  -DDEVICE=tanmatsu \
  '-DSDKCONFIG_DEFAULTS=sdkconfigs/general;sdkconfigs/tanmatsu' \
  -DSDKCONFIG=sdkconfig_tanmatsu \
  -DIDF_TARGET=esp32p4 reconfigure

idf.py -B build/tanmatsu build
```

Run the activation and build as separate commands. The installed activation
script may return a nonzero status after successfully configuring the shell;
the launcher's Makefile chains activation with `&&`, which can prevent its
build command from running. The direct commands above avoid that issue.

This downloads the pinned components and verifies the SDK by building the
launcher. It does not flash or erase the device. Do not use `make refreshsdk`
for this update: that target deletes the configured SDK/tools directories.
If `git pull --ff-only` reports divergent history, stop and inspect the branches;
do not reset away local commits.

The isolated build tested for this weather plugin used upstream launcher commit
`20022fc` and ESP-IDF v6.0.2. The older local commit `7043218` has an add-on/EEPROM
dependency mismatch; the upstream update advances the add-on component to 0.1.0
and the BSP to 1.4.0, resolving that build failure.

## Add weather support after the update

The stock launcher does not yet export the weather extension. From the launcher
directory, apply the supplied patch from your Weather source directory:

```bash
git apply --check /home/siv/Development/Plugins/Weather/launcher.patch
git apply /home/siv/Development/Plugins/Weather/launcher.patch
idf.py -B build/tanmatsu build
```

The patch was also confirmed to apply to the older local source, but update
first to obtain the dependency fix. If the check fails on a later upstream
version, the patch needs rebasing; do not force it.

Installing the built launcher onto the device is a separate step. No device
flash has been performed by this task.
