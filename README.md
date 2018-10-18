# PictureCrypt
Make your pictures crypted.

[![Build Status](https://travis-ci.com/waleko/PictureCrypt.svg?branch=master)](https://travis-ci.com/waleko/PictureCrypt)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0c1f3e2bd51c4feebf5a4ce9d2692660)](https://app.codacy.com/app/waleko/PictureCrypt?utm_source=github.com&utm_medium=referral&utm_content=waleko/PictureCrypt&utm_campaign=Badge_Grade_Dashboard)

## About
Project is made only using QT.
[QAESEncryption](https://github.com/bricke/Qt-AES) by bricke was also used.
MVC pattern used.
PictureCrypt project is UI based, the model contains all buisness logic and can work as standalone class.

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
									int mode = 0, // Mode of embedding
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
* 0 - Standard, created by me
* 1 - JPHS, requires manually installed JPHS and specified directory (not currently available).

## Documentation
Doxygen ocumentation available [here](https://alexkovrigin.me/PictureCrypt)

PDF documentation available [here](https://github.com/waleko/PictureCrypt/raw/gh-pages/refman.pdf)

## Dependencies
* qtcore
* qtgui
* qtwidgets

## Works of other people used in this project
 * [QAESEncryption](https://github.com/bricke/Qt-AES) by bricke (provided under [UNLICENSE](https://unlicense.org/))

## Contact
Question or suggestions are welcome!
Email me a.kovrigin0@gmail.com or visit my site https://alexkovrigin.me

## License
This software is provided under the [UNLICENSE](http://unlicense.org/)
