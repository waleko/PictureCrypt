# PictureCrypt
Make your pictures crypted.


## About
Project is made only using QT.
[QAESEncryption](http://github.com/bricke/Qt-AES) by bricke was also used.
MVC pattern used.
PictureCrypt project is UI based, the model contains all buisness logic and can work as standalone class.

## External use
ModelPC class can be used externally (without UI)
```
#include <modelpc.h>
#include <QByteArray>
#include <QImage>

...

ModelPC * model = new ModelPC(ver);
// ver is version of the app, used to check the data structure version
// ver is type long and is calculated as if version is "x.y.z" => ver = x * 65536 + y * 256 + z
// Default parameter is 2^17 (2.0.0)

// Connecting signals

// Essential ones

model->saveData(QByteArray data)
// Used to return the retrieved data

model->saveImage(QImage * image)
// Used to return the modified image

// Extra ones

model->alertView(QString message, bool isWarning)
// Used for messages to be shown to users

model->setProgress(int val)
// Used to show user the progress of embedding
// -1 indicates the creation of some kind of progress dialog
// from 0 to 100 shows the progress
// 101 indicates that progress dialog should be closed

```

## Avaible methods
### Essential ones
#### start
Used for embedding

Parameters:
data	Data to be encrypted
_image	Image to be encrypted into.
_bitsUsed	Bits per byte, see also ModelPC::bitsUsed
key	Key, if default (empty), random key of 64 charachters will be generated.
mode	Mode of encryption
```
model->start(QByteArray data, QImage image, int mode = 0, QString key = "", int _bitsUsed = 8);
```

#### decrypt
Used for de-embedding

Parameters:
image	Image to be decrypted.

```
model->decrypt(QImage * image);
```
### Extra ones
#### encrypt
Used for embedding but with data already packed with stuff like version, file size, aes key, etc.
Used in PictureCrypt project

Parameters:

encr_data	Data to be embbed to an image.
image	Image to be embbed into.
mode	Mode of encryption

```
model->encrypt(QByteArray encr_data, QImage * image, int mode = 0);
```
#### fail
Used for stopping the embedding or de-embedding proccess
Parameters:

message    Message for user
```
model->fail(QString message);
```

## Available modes of embedding
* 0 - Standard, created by me
* 1 - JPHS, requires manually installed JPHS and specified directory (not currently available).

## Documentation
Doxygen Documentation avaible [here](https://waleko.github.io/doc/picturecrypt)


## Dependencies
* qtcore
* [QAESEncryption](https://github.com/bricke/Qt-AES) by bricke

## Contact
Question or suggestions are welcome!
Please use the GitHub issue tracking to report suggestions or issues.
Email me a.kovrigin0@gmail.com and visit my site http://alex.unaux.com

## License
This software is provided under the [UNLICENSE](http://unlicense.org/)
