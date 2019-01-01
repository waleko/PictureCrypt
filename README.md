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
ModelPC class can be used externally (without UI)
```
#include <modelpc.h>
#include <testpc.h>
#include <QByteArray>
#include <QImage>

...

// Testing the ModelPC
if(TestPC::Test())
	return;
ModelPC * model = new ModelPC();

// Embedding
QImage * resultImage = model->start(QByteArray data, // Data to be embedded
									QImage *image, // Image for embedding
									int mode, // Mode of embedding
									QString key = "", // Key for extra-encryption (is required, if not given, error will show up)
									int bitsUsed = 8, // Bits per Byte used (better explaination ModelPC::bitsUsed)
									QString *error = nullptr); // Error output, if everything is ok, error will be "ok"
if(*error != "ok")
	return;
// Note *error is just a code of error (like "muchdata", dictionary of error codes is also available on github.

// De-embedding
QByteArray output = model->decrypt(QImage * image, // Image with hidden data
									 QString key = "" // Key for extra-encryption
								   QString *_error = nullptr); // Error output
if(data == output)
	qDebug() << "Great success!";
else
	qDebug() << "Fiasco :(";
```

## Available modes of embedding
* 0 - Not Defined, used for decryption, so it auto-detects (invalid on encryption as you must select the encryption type).
* **1** - v1.3, only one available on versions 1.3+, pretty basic, but stable.
* 2 - v1.4, advanced encryption mode, available on versions v1.4+.
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
