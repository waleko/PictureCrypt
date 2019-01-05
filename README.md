# PictureCrypt
Make your pictures crypted.

[![Build Status](https://travis-ci.com/waleko/PictureCrypt.svg?branch=master)](https://travis-ci.com/waleko/PictureCrypt)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0c1f3e2bd51c4feebf5a4ce9d2692660)](https://app.codacy.com/app/waleko/PictureCrypt?utm_source=github.com&utm_medium=referral&utm_content=waleko/PictureCrypt&utm_campaign=Badge_Grade_Dashboard)

## About
A simple steganography project which hides data in images
This project is built using MVC pattern and features GUI.
[Qt](https://qt.io) and [QAESEncryption](https://github.com/bricke/Qt-AES) by [bricke](https://github.com/bricke) were used.

## Download
Get the binary files at [latest release page](https://github.com/waleko/PictureCrypt/releases/latest)
Or download latest **UNSTABLE** binary file for linux [here](https://github.com/waleko/PictureCrypt/raw/gh-pages/src/build/Release/PictureCrypt)

## External use
You can use ModelPC class separately from everything else, except for QAESEncryption (so /aes folder)
### Use API
```
// Includes
#include <modelpc.h>
#include <QImage>
#include <QByteArray>
#include <QString>
#include <QDebug> // just for showcase

// Basic setup
QByteArray data("some_file.txt");
QImage *image = new QImage("some_big_enough_image.jpg");
QString key = "some_password";
int bitsUsed = 1; // must be from 1 to 8

// Encrypting
QString error1, error2;
QImage *normal_resultImage = ModelPC::Encrypt(data, image, 1, key, bitsUsed, &error1);
QImage *advanced_resultImage = ModelPC::Encrypt(data, image, 2, key, bitsUsed /* not really used here, so put here any number from 1 to 8*/, &error2);

// Decrypting with given mode
QString error3, error4, error5, error6;
QByteArray output_normal = ModelPC::Decrypt(normal_resultImage, key, 1, &error3);
QByteArray output_advanced = ModelPC::Decrypt(advanced_resultImage, key, 2, &error4);

// Decrypting without given mode
// PictureCrypt can detect the mode of the image and adapt.
QByteArray output_normal_undefined = ModelPC::Decrypt(normal_resultImage, key, 0, &error5);
QByteArray output_advanced_undefined = ModelPC::Decrypt(advanced_resultImage, key, 0, &error6);

// Check
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
### Tests
PictureCrypt comes with QT tests ready
#### Run tests
* Run them directly from IDE (e.g. Qt Creator)

## Available modes of embedding
* 0 - Not Defined, used for decryption, so it auto-detects (invalid on encryption as you must select the encryption type).
* 1 - v1.3, only one available on versions 1.3+, pretty basic.
* **2** - v1.4, advanced encryption mode, available on versions v1.4+ (works a lot longer than v1.3, can work for >40s on slow machines).
* 3 - JPHS, requires manually installed JPHS and specified directory (not currently available).

## Documentation
Doxygen documentation available [here](https://alexkovrigin.me/PictureCrypt)

PDF documentation available [here](https://github.com/waleko/PictureCrypt/raw/gh-pages/refman.pdf)

## Dependencies
* [qtcore](https://doc.qt.io/qt-5.11/qtcore-index.html)
* [qtgui](http://doc.qt.io/archives/qt-4.8/qtgui-module.html)
* [qtwidgets](https://doc.qt.io/qt-5.11/qtwidgets-index.html)

## Works of other people used in this project
 * [QAESEncryption](https://github.com/bricke/Qt-AES) by [bricke](https://github.com/bricke/) (provided under [UNLICENSE](https://unlicense.org/))

## Contact
Question or suggestions are welcome!
Email me a.kovrigin0@gmail.com or visit my site https://alexkovrigin.me

## License
This software is provided under the [UNLICENSE](http://unlicense.org/)
