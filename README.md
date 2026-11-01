# Application repository

To upload an application fork this repository and create a pull request in which you add a folder for your app.

The name of your app must clearly indicate its author by referencing a reversed URL name.

_For example if your website is example.com and the name of your app is test then your folder name becomes com.example.test._

We refer to the name of this folder as the `slug` name of your app.

Put all the relevant files (firmware binaries and assets) in your app folder and add a file called `metadata.json`.

The metadata file should contain an object with information about your app.

 - `name`: the name of your app as a human readable string
 - `description`: a short description of your app, will be displayed on small screens so keep it short
 - `repository_url`: the URL at which people can find the repository that contains the source code of your app (optional)
 - `categories`: an array of categories in which your application fits
 - `version`: version number of your app, change to trigger an update of your app on devices which have it installed
 - `icon`: an object containing string entries for each available icon size. We recommend at least providing `16x16`, `32x32` and `64x64` icons in `png` format.
 - `author`: your (company) name
 - `license_type`: the [SPDX identifier](https://spdx.org/licenses/) for the license of your app or `proprietary` for other licence terms, specified in the license file provided
 - `license_file`: the filename of a file containing the full license text of your app (optional)
 - `application`: an array of objects describing which target devices your app supports and how your app should be installed onto each target device

Each object in the application array must contain the following entries:

 - `targets`: an array containing the names of supported boards, such as `tanmatsu`, `konsool` or `why2025`. Since Tanmatsu and Konsool are essentially the same device we recommend always adding both to apps that have been developed for one of these boards.
 - `type`: the application type. Supported values are `appfs` (a binary executed directly from the application filesystem) and `script` (an application run by another installed app acting as an interpreter, specified via `interpreter`)
 - `revision`: an integer revision number for this build of your app, change to trigger an update of your app on devices which have it installed (optional)
 - `bsp_version`: the major BSP version used to build this application, used to indicate support for newer hardware such as the updated Tanmatsu display (optional, place it directly below `revision`)
 - `executable`: the firmware binary to install
 - `interpreter`: the `slug` of another installed app that acts as the interpreter for this application, required when `type` is `script`
 - `assets`: an array containing a list of objects, each object represents a file that will be installed to either the internal FAT partition or to an inserted SD card, if you target a board which supports SD cards be sure to check both the internal FAT filesystem and the SD card for your files. The files will be copied to the `<mountpoint>/apps/<slug>/` folder. The object should be formatted as `{"source_file": "source_file.txt", "target_file": "destination_file.txt"}`. You may of course use the same name for both source and destination, but this approach allows for example for having one of multiple source files be copied to one destination depending on the target device.
 - `external_only`: when true, only allow installation onto external storage media such as an SD card, not internal memory (optional)
 - `external_preferred`: when true, prefer installation onto external storage media such as an SD card (optional)
 - `internal_only`: when true, only allow installation onto internal storage (optional)
 - `internal_preferred`: when true, prefer installation onto internal storage (optional)

Example:

```
{
  "name": "My example app",
  "description": "Saying hello to the world",
  "repository_url": "https://github.com/johnsmith/my-example-app",
  "categories": [
    "communication"
  ],
  "version": "0.0.1",
  "icon": {
    "16x16": "icon16.png",
    "32x32": "icon32.png",
    "64x64": "icon64.png"
  },
  "author": "John Smith",
  "license_type": "MIT",
  "license_file": "LICENSE",
  "application": [
    {
      "targets": [
        "tanmatsu",
        "konsool"
      ],
      "revision": 1,
      "bsp_version": 1,
      "type": "appfs",
      "executable": "myawesomeapp.bin",
      "assets": [
        {
          "source_file": "someresource.png",
          "target_file": "someresource.png"
        }
      ]
    }
  ]
}
```
## Note on licensing

By agreeing to the contributors license agreement you (among many other grants) grant us (`Nicolai Electronics`) the right to distribute your application, regardless of the terms specified in the license file, as specified in section 2.1 of the CLA.
