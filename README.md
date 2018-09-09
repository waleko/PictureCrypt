# PictureCrypt
Make your pictures crypted.

[![Build Status](https://travis-ci.com/waleko/PictureCrypt.svg?branch=master)](https://travis-ci.com/waleko/PictureCrypt)

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
TestPC testing;
if(!testing.startTest())
	return;
ModelPC * model = new ModelPC();

// Embedding
QImage * resultImage = model->start(QByteArray data, // Data to be embedded
									QImage *image, // Image for embedding
									int mode = 0, // Mode of embedding
									QString key = "", // Key for extra-encryption (if empty, key will be generated automatically)
									int bitsUsed = 8, // Bits per Byte used (better explaination ModelPC::bitsUsed)
									QString *error = nullptr); // Error output, if everything is ok, error will be "ok"
if(*error != "ok)
	return;
// Note *error is just a code of error (like "muchdata", dictionary of error codes is also available on github.

// De-embedding
QByteArray output = model->decrypt(QImage * image, // Image with hidden data
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
Doxygen Documentation avaible [here](https://alexkovrigin.me/PictureCrypt)

## Dependencies
* qtcore
* qtgui
* qtwidgets

## Works of other people used in this project
 * [QAESEncryption](https://github.com/bricke/Qt-AES) by bricke (provided under [UNLICENSE](https://unlicense.org/))

## Contact
Question or suggestions are welcome!
Please use the GitHub issue tracking to report suggestions or issues.
Email me a.kovrigin0@gmail.com and visit my site https://alexkovrigin.me

## License
This software is provided under the [UNLICENSE](http://unlicense.org/)
