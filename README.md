<p align="center">
  <a href="https://alexkovrigin.me/PictureCrypt">
    <img alt="PictureCrypt" src="./src/app/icons/unlocked.png">
  </a>
</p>

<p align="center">
  An image steganography tool
</p>
<p align="center">
  <a href="https://travis-ci.com/waleko/PictureCrypt"><img alt="Build Status" src="https://travis-ci.com/waleko/PictureCrypt.svg?branch=master"></a>
  <a href="https://ci.appveyor.com/project/waleko/picturecrypt/branch/master"><img src="https://ci.appveyor.com/api/projects/status/qc0syxtjax4wnud7/branch/master?svg=true"/></a>
  <a href="https://ci.appveyor.com/project/waleko/picturecrypt/branch/master/tests"><img alt="undefined" src="https://img.shields.io/appveyor/tests/waleko/PictureCrypt/master.svg?style=flat"></a>
  <!--<a class="badge-align" href="https://www.codacy.com/app/waleko/PictureCrypt?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=waleko/PictureCrypt&amp;utm_campaign=Badge_Grade"><img src="https://api.codacy.com/project/badge/Grade/c9106eb67e164d7d87de6d92448a3355"/></a>-->
  <!--<img alt="Chocolatey Version" src="https://img.shields.io/chocolatey/v/picturecrypt">-->
  <a href="https://waleko.github.io/PictureCrypt/docs"><img alt="Documentation" src="https://img.shields.io/badge/docs-available-brightgreen.svg"></a>
  <a href="https://github.com/waleko/PictureCrypt/blob/master/LICENSE"><img alt="License" src="https://img.shields.io/github/license/waleko/PictureCrypt.svg?style=flat"></a>
  <!--<a href="https://gitter.im/PictureCrypt/community"><img src="https://badges.gitter.im/waleko/PictureCrypt.png"/></a>-->
</p>


```cpp
// Encryption
QByteArray message("secret");
QImage *image = new QImage("./images/sunset.jpg");
QImage *image_to_send = ModelPC::Encrypt(message, image, "password1");

// Decryption
QByteArray incoming_message = ModelPC::Decrypt(image_to_send, "password1"); // "secret"
```

<p align="center">
  <img src="https://waleko.github.io/data/picturecrypt-screenshot.jpg"/>
</p>

## What is PictureCrypt
PictureCrypt is a simple educational cross-platform steganography tool that hides data in images.
This project is built on [Qt Platform](https://qt.io) using the MVC pattern and features GUI and console interface.
Project repository is available on [GitHub](https://github.com/waleko/PictureCrypt).

## Download
Get stable binary files at [latest release page](https://github.com/waleko/PictureCrypt/releases/latest)
Or download latest **potentially unstable** files:

### Links
##### Windows:
* [**Exe with release**](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/src/PictureCrypt-setup.exe) (~ 22 MB)
* [**Zip with console**](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/src/deploy/console.zip) (~ 21 MB)
* [Exe with console](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/src/PictureCrypt-console-setup.exe) (~ 19 MB)

##### Linux:
* [Binary of release](https://github.com/waleko/PictureCrypt/raw/gh-pages/src/app/build/Release/PictureCrypt) (**Qt not included**) (~ 10 MB)


## Console use
```bash
$ picturecrypt -h
Usage:
  picturecrypt encrypt <image> <input file> <key> <output> [options]
  picturecrypt decrypt <image> <key> <output> [options]
  picturecrypt (-h | --help)
  picturecrypt --version

Options:
  -h --help    Show this screen.
  --version    Show version.
  -m --mode    Mode of cryption.
  -b           Encryption bitsUsed parameter.
$ picturecrypt --version
1.4.1
$ picturecrypt encrypt original.jpg data.txt somekey result.png
$ picturecrypt decrypt result.png somekey output.txt
```

## Available modes of encrypting
* 0 - Auto-detect, used for decryption, so it auto-detects (invalid at encryption as you must select the encryption type)
* 1 - v1.3, only available on versions 1.3+
* **2** - v1.4, advanced (default) encryption mode, available on versions v1.4+

## Documentation
* [HTML documentation](docs)
* [PDF documentation](https://github.com/waleko/PictureCrypt/raw/gh-pages/PictureCrypt-docs.pdf)

Documentation generated by [Doxygen](http://doxygen.nl)

## CI/CD
Continuous integration is fulfilled via [Travis CI](https://travis-ci.com/waleko/PictureCrypt) for Linux and [AppVeyor](https://ci.appveyor.com/project/waleko/picturecrypt) for Windows.

## i18n
### Languages
This project is available in multiple languages thanks to the Qt Linguist Platform.

Available languages:

- English (default)
- Russian
- German

### Help translating
If you would like to translate PictureCrypt to your language here are the steps to do so:

* Install Qt and Qt Linguist with it
* Clone the project.
* Go to PictureCrypt/src/app/
* Add a filename to `TRANSLATIONS` in src/app/app.pro file. Filename must be 'picturecrypt_{your language}.ts' (e.g. 'picturecrypt_fr.ts')
* Run `lupdate src.pro`, it will generate that .ts file. An error may occur: "Maybe you forgot to set your environment?" To solve it go to Project (on the left-hand side) ->(expand) Build environment -> Open terminal and run `lupdate src.pro`.
* Go to translations/
* Run `linguist picturecrypt_{your language}.ts`. The Qt Linguist will appear, where you can translate given text from English (there are a couple of html entries, just edit its text part).
* Either go to 'File -> Compile' or run `lrelease picturecrypt_{your language}.ts`.
* Add your picturecrypt_{your language}.ts to src/app/translations.qrc resource file.
* Create a pull request with your work.

## Dependencies
* [qtcore](https://doc.qt.io/qt-5.11/qtcore-index.html)
* [qtgui](http://doc.qt.io/archives/qt-4.8/qtgui-module.html)
* [qtwidgets](https://doc.qt.io/qt-5.11/qtwidgets-index.html)

## Works of other people used in this project
 * [QAESEncryption](https://github.com/bricke/Qt-AES) by [bricke](https://github.com/bricke/) (provided under [unlicense](https://unlicense.org/))
 * 'Circle Icons' by Nick Roach provided under [GPL v3.0](https://www.gnu.org/licenses/gpl-3.0.html).
 * 'MS Shell Dlg 2' font used and 'Montserrat' font used under the SIL Open Font License.

## Contact
Questions and suggestions are welcome!
Email me a.kovrigin0@gmail.com or telegram me at [waleko](https://t.me/waleko)

## License
This software [MIT licensed](./LICENSE).

Copyright © 2021 Alexander Kovrigin
