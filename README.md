<p align="center">
  <a href="https://alexkovrigin.me/PictureCrypt">
    <img alt="PictureCrypt" src="./app/src/icons/unlocked.png">
  </a>
</p>

<p align="center">
  An image-steganography project
</p>
<p align="center">
  <a href="https://travis-ci.com/waleko/PictureCrypt"><img alt="Build Status" src="https://travis-ci.com/waleko/PictureCrypt.svg?branch=master"></a>
  <a href="https://ci.appveyor.com/project/waleko/picturecrypt/branch/master"><img src="https://ci.appveyor.com/api/projects/status/qc0syxtjax4wnud7/branch/master?svg=true"/></a>
  <a class="badge-align" href="https://www.codacy.com/app/waleko/PictureCrypt?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=waleko/PictureCrypt&amp;utm_campaign=Badge_Grade"><img src="https://api.codacy.com/project/badge/Grade/c9106eb67e164d7d87de6d92448a3355"/></a>
  <a href="https://gitter.im/PictureCrypt/community"><img src="https://badges.gitter.im/waleko/PictureCrypt.png"/></a>
</p>

<p align="center">
  <img src="https://waleko.github.com/data/picturecrypt-screenshot.jpg"/>
</p>

## About
A simple cross-platform steganography project which hides data in images.
This project is built on [Qt Platform](https://qt.io) using MVC pattern and features GUI and console interface.

## Download
Get stable binary files at [latest release page](https://github.com/waleko/PictureCrypt/releases/latest)
Or download latest potentially **UNSTABLE** files:

### Links
#### Windows:
* [**EXE with release**](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/app/PictureCrypt-setup.exe) (~ 22 MB)
* [**Zip with console**](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/app/deploy/console.zip) (~ 21 MB)
* [Zip Debug](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/app/deploy/PictureCrypt-debug.zip) (~ 190 MB)
* [Zip Release](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/app/deploy/PictureCrypt-release.zip) (~ 21 MB)
* [Exe with console](https://ci.appveyor.com/api/projects/waleko/picturecrypt/artifacts/app/PictureCrypt-console-setup.exe) (~ 19 MB)

#### Linux:
* [Binary of release](https://github.com/waleko/PictureCrypt/raw/gh-pages/app/src/build/Release/PictureCrypt) (**doesn't work without QT**) (~ 10 MB)

## External use
You can use ModelPC class separately from everything else, **except for QAESEncryption** (so /aes folder)

### Use API
```cpp
// Includes
#include "modelpc.h"
#include <QImage>
#include <QByteArray>
#include <QString>
#include <QDebug> // just for showcase

...

// Basic setup
QByteArray data("some_file.txt");
QImage *image = new QImage("some_big_enough_image.jpg");
QString key = "some_password";
int bitsUsed = 3; // must be from 1 to 8

// Encrypting
QString error1, error2;
QImage *normal_resultImage = ModelPC::Encrypt(
        data,
        image,
        1,
        key,
        bitsUsed,
        &error1);
QImage *advanced_resultImage = ModelPC::Encrypt(
        data,
        image,
        2, key,
        bitsUsed /* not really used here, so put here any number from 1 to 8*/,
        &error2);

// Decrypting with given mode
QString error3, error4, error5, error6;
QByteArray output_normal = ModelPC::Decrypt(
        normal_resultImage,
        key,
        1,
        &error3);
QByteArray output_advanced = ModelPC::Decrypt(
        advanced_resultImage,
        key,
        2,
        &error4);

// Decrypting without given mode
// PictureCrypt can detect the mode of the image and adapt.
QByteArray output_normal_undefined = ModelPC::Decrypt(
        normal_resultImage,
        key,
        0,
        &error5);
QByteArray output_advanced_undefined = ModelPC::Decrypt(
        advanced_resultImage,
        key,
        0,
        &error6);

// Check (better testing with [running tests](#run-tests)
bool data_good =
        data == output_normal &&
        data == output_advanced &&
        data == output_normal_undefined &&
        data == output_advanced_undefined;
bool no_errors =
        error1 == "ok" &&
        error2 == "ok" &&
        error3 == "ok" &&
        error4 == "ok" &&
        error5 == "ok" &&
        error6 == "ok";
if(data_good && no_errors)
    qDebug() << "PASS";
else
    qDebug() << "FAIL";

```

### Console use
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

### Tests
PictureCrypt comes with Qt Test project. It can be found at 'app/tests'

#### Run tests
* Run them directly from IDE (e.g. Qt Creator)
* Run with 'make'

```bash
# Go to tests directory
cd app/tests

# Install required packages
bash ../scripts/install.sh

# Build tests
bash ../scripts/build.sh

# Run tests
QT_QPA_PLATFORM=offscreen make check

# Clean (optional)
bash ../scripts/clean.sh
```

## Available modes of encrypting
* 0 - Not Defined, used for decryption, so it auto-detects (invalid on encryption as you must select the encryption type).
* 1 - v1.3, only one available on versions 1.3+, pretty basic.
* **2** - v1.4, advanced (preferred) encryption mode, available on versions v1.4+ (works a lot longer than v1.3, can work for >40s on slow machines).
* 3 - JPHS, requires manually installed JPHS and specified directory (not currently available).

## Documentation
* HTML documentation available [here](docs)
* PDF documentation available [here](https://github.com/waleko/PictureCrypt/raw/gh-pages/refman.pdf)

## Continuous integration
Continuous integration is fulfilled via [Travis CI](https://travis-ci.com/waleko/PictureCrypt) for Linux and [AppVeyor](https://ci.appveyor.com/project/waleko/picturecrypt) for Windows.

## Internationalization
### Languages
This project is available in multiple languages thanks to awesome QT Linguist Platform.
Currently we have:

- English (default)
- Russian
- German

### Help translating
If you'd like to translate PictureCrypt to your language here are steps to do so:

* Install QT and QT Linguist with it
* Clone the project.
* Go to PictureCrypt/app/src/
* Add a filename to TRANSLATIONS in app/src/src.pro file. Filename must be 'picturecrypt_{your language}.ts' (e.g. 'picturecrypt_fr.ts')
* Run `lupdate src.pro`, it will generate that .ts file. If there is an error: "Maybe you forgot to set your environment?" Go to Project (on the left-hand side) ->(expand) Build environment -> Open terminal and run `lupdate src.pro`.
* Go to translations/
* Run `linguist picturecrypt_{your language}.ts`. The Qt Linguist will appear, where you can translate given text from English (there are a couple of html entries, just edit its text part). **If you don't translate the whole thing, your work will be still appreciated!**
* Either go to 'File -> Compile' or run `lrelease picturecrypt_{your language}.ts`.
* Add your picturecrypt_{your language}.ts to app/src/translations.qrc resource file.
* Create a pull request in GitHub with your work.

If you have any trouble, [contact me](#contact) and I will sincerely try to help you as I really don't think, that anyone will decide to help me translate. 😔

## Dependencies
* [qtcore](https://doc.qt.io/qt-5.11/qtcore-index.html)
* [qtgui](http://doc.qt.io/archives/qt-4.8/qtgui-module.html)
* [qtwidgets](https://doc.qt.io/qt-5.11/qtwidgets-index.html)

## Works of other people used in this project
 * [QAESEncryption](https://github.com/bricke/Qt-AES) by [bricke](https://github.com/bricke/) (provided under [UNLICENSE](https://unlicense.org/))
 * 'Circle Icons' by Nick Roach provided under GPL v3.0.
 * 'MS Shell Dlg 2' font used and 'Montserrat' font used under the SIL Open Font License.

## Contact
Questions and suggestions are welcome!
Email me [a.kovrigin0@gmail.com](mailto:a.kovrigin0@gmail.com) or telegram me at [waleko](https://t.me/waleko)

## License
This software is provided under the [UNLICENSE](http://unlicense.org/)
