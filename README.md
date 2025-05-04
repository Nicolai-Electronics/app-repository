# Application repository

To upload an application fork this repository and create a pull request in which you add a folder for your app.

The name of your app must clearly indicate its author by referencing a reversed URL name.

_For example if your website is example.com and the name of your app is test then your folder name becomes com.example.test._

We refer to the name of this folder as the `slug` name of your app.

Put all the relevant files (firmware binaries and assets) in your app folder and add a file called `metadata.json`.

The metadata file should contain an object with information about your app.

 - `name`: the name of your app as a human readable string
 - `description`: a short description of your app, will be displayed on small screens so keep it short
 - `categories`: an array of categories in which your application fits
 - `version`: version number of your app, change to trigger an update of your app on devices which have it installed
 - `icon`: an object containing string entries for each available icon size. We recommend at least providing `16x16`, `32x32` and `64x64` icons in `png` format.
 - `author`: your (company) name
 - `license`: the license for your app
 - `application`: an array of objects describing which target devices your app supports and how your app should be installed onto each target device

Each object in the application array must contain the following entries:

 - `target`: an array containing the names of supported boards, currently only `tanmatsu` and `konsool` are supported entries. Since Tanmatsu and Konsool are essentially the same device we recommend always adding both to apps that have been developed for one of these boards.
 - `type`: the application type, currently only `appfs` is supported
 - `executable`: the firmware binary to install
 - `assets`: an array containing a list of filenames as string, any files listed like this will be installed to either the internal FAT partition or to an inserted SD card, if you target a board which supports SD cards be sure to check both the internal FAT filesystem and the SD card for your files. The files will be copied to the `<mountpoint>/apps/<slug>/` folder.


