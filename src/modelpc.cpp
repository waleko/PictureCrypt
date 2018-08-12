#include "modelpc.h"
#include <QDebug>
/*!
 * \brief ModelPC::ModelPC Constructor
 * Unit tests are run here.
 * \sa ControllerPC, ViewPC
 */
ModelPC::ModelPC()
{
    // Version control
    versionString = "1.3.0";

    auto ver = versionString.split(".");
    version = ver[0].toInt() * pow(2, 16) + ver[1].toInt() * pow(2, 8) + ver[2].toInt();

    ver_byte = bytes(ver[0].toInt()) +
            bytes(ver[1].toInt()) +
            bytes(ver[2].toInt());
    // Random seed
    qsrand(randSeed());
}
/*!
 * \brief ModelPC::start Slot to zip and encrypt data and provide it with some extra stuff
 * After completion start standard ModelPC::encrypt
 * Isn't used in PictureCrypt, but used can be used in other - custom projects.
 * \param data Data for embedding
 * \param image Image for embedding
 * \param mode Mode for embedding
 * \param key Key for extra encryption (if empty, key will be auto-generated)
 * \param _bitsUsed Bits per byte (see ModelPC::bitsUsed)
 * \param _error Error output
 * \return Returns image with embedded data
 * \sa ModelPC::encrypt
 */
QImage * ModelPC::start(QByteArray data, QImage * image, int mode, QString key, int _bitsUsed, QString *_error)
{
    // Error management
    if(_error == nullptr)
        _error = new QString();
    *_error = "ok";
    error = _error;

    if(data.isEmpty()) {
        fail("nodata");
        return nullptr;
    }
    if(image == nullptr || image->isNull()) {
        fail("nullimage");
        return nullptr;
    }
    if(_bitsUsed < 1 || _bitsUsed > 8) {
        fail("bitsWrong");
        return nullptr;
    }
    if(key.isEmpty()) {
        qsrand(randSeed());
        for(int i = 0; i < 32; i++)
            key.append(48 + qrand() % 75);
    }
    else if(key.size() > 255) {
        fail("bigkey");
        return nullptr;
    }
    long long usedBytes = data.size() + 14 + key.size();
    long long size = image->width() * image->height();
    if(usedBytes * 100 / (size * 3) * 8 / _bitsUsed > 70) {
        fail("muchdata");
        return nullptr;
    }

    curMode = mode;
    bitsUsed = _bitsUsed;

    QByteArray key_data = key.toUtf8();
    QByteArray zipped_data = zip(data, key_data);
    QByteArray encr_data = bytes(key_data.size()) + key_data + zipped_data;

    if(*error == "ok")
        return encrypt(encr_data, image, curMode, error);
    else
        return nullptr;
}

/*!
 * \brief ModelPC::encrypt Slot to be called when encrypt mode in ViewPC is selected and started.
 * \param encr_data Data to be inserted to an image.
 * \param image Image to be inserted in.
 * \param mode Mode of encryption
 * \param _error Error output
 * \return Returns image with embedded data.
 * \sa ViewPC::on_startButton_clicked, ModelPC::decrypt, ModelPC::circuit, ModelPC::start
 */
QImage * ModelPC::encrypt(QByteArray encr_data, QImage * image, int mode, QString *_error)
{
    // Error management
    if(_error == nullptr)
        _error = new QString();
    *_error = "ok";
    error = _error;

    // TODO Remove debug mode = 0
    mode = 0;

    if(encr_data.isEmpty()) {
        fail("nodata");
        return nullptr;
    }
    if(image == nullptr || image->isNull()) {
        fail("nullimage");
        return nullptr;
    }

    encr_data = ver_byte + encr_data;
    long long int countBytes = encr_data.size();
    curMode = mode;
    switch(curMode)
    {
    case 0:
        circuit(image, &encr_data, countBytes);
        break;
    case 1:
        jphs(image, &encr_data);
        break;
    default:
        fail("wrongmode");
        return nullptr;
    }


    // Saving
    if(success) {
        emit saveImage(image);
        return image;
    }
    else
        return nullptr;
}
/*!
 * \brief ModelPC::decrypt Slot to be called when decrypt mode in ViewPC is selected and started.
 * \param image Image to be decrypted.
 * \return Returns decrypted data
 * \param _error Error output
 * \sa ViewPC::on_startButton_clicked, ModelPC::encrypt, ModelPC::circuit
 */
QByteArray ModelPC::decrypt(QImage * image, QString *_error)
{
    // Error management
    if(_error == nullptr)
        _error = new QString();
    *_error = "ok";
    error = _error;
    if(image == nullptr || image->isNull()) {
        fail("nullimage");
        return nullptr;
    }
    // Image opening
    int w = image->width();
    int h = image->height();

    // Getting corner pixels
    QColor colUL = image->pixelColor(0, 0).toRgb();
    QColor colUR = image->pixelColor(w - 1, 0).toRgb();
    QColor colDR = image->pixelColor(w - 1, h - 1).toRgb();

    // Getting verification code
    int verifCode = (((colUR.green() % 2) << 5) + colUR.blue() % 32) << 2;
    verifCode += colDR.blue() % 4;
    if(verifCode != 166){
        fail("veriffail");
        return nullptr;
    }
    // Getting number of bytes
    long long int countBytes = (colUL.blue() % 32 + ((colUL.green() % 32) << 5) + ((colUL.red() % 32) << 10)) << 9;
    countBytes += ((colUR.red() % 32) << 4) + (colUR.green() >> 1) % 16;

    bitsUsed = (colDR.blue() >> 2) % 8 + 1;
    curMode = colDR.green() % 32;

    // Start of the circuit
    QByteArray data;
    circuit(image, &data, countBytes);

    // Check if circuit was successful
    if(!success)
        return nullptr;
    if(data.isEmpty())
    {
        fail("noreaddata");
        return nullptr;

    }
    // Version check
    long long int _ver = mod(data.at(0) * pow(2, 16));
    _ver += mod(data.at(1) * pow(2, 8));
    _ver += mod(data.at(2));
    data.remove(0, 3);
    if(_ver > version) {
        fail("Picture's app version is newer than yours. Image version is "
              + generateVersionString(_ver) + ", yours is "
              + generateVersionString(version) + ".");
        return nullptr;
    }
    else if(_ver < version) {
        fail("Picture's app version is older than yours. Image version is "
              + generateVersionString(_ver) + ", yours is "
              + generateVersionString(version) + ".");
        return nullptr;
    }

    // Obtain the key
    int key_size = mod(data.at(0));
    QByteArray key = data.mid(1, key_size);
    data.remove(0, key_size + 1);

    // Unzip
    QByteArray unzipped_data = unzip(data, key);
    emit saveData(unzipped_data);
    return unzipped_data;
}
/*!
 * \brief ModelPC::fail Slot to stop execution of cryption
 * \param message Message for user
 */
void ModelPC::fail(QString message)
{
    *error = message;
    alert(message, true);
    success = false;
    emit setProgress(101);
}
/*!
 * \brief ModelPC::jphs JPHS function to use jphide and jpseek (currently under development)
 * \param image Image for embedding
 * \param data Data
 */
void ModelPC::jphs(QImage *image, QByteArray *data)
{
    // Under Development
    return;

    // Dead code

    success = true;
    bool isEncrypt = !data->isEmpty();
    QString targetEXE = defaultJPHSDir + (isEncrypt ? "/jphide.exe" : "/jpseek.exe");
    if(!fileExists(targetEXE))
    {
        fail("nojphs");
        return;
    }

    QString randomFileName = defaultJPHSDir + "/";
    qsrand(randSeed());
    for(int i = 0; i < 10; i++)
        randomFileName.append(97 + qrand() % 25);
    image->save(randomFileName + ".jpg");
    if(isEncrypt) {
        QFile file(randomFileName + ".pc");
        if(!file.open(QFile::WriteOnly)) {
            fail("savefilefail");
            return;
        }
        file.write(*data);
        file.close();

        QStringList args;
        args << (randomFileName + ".jpg") << (randomFileName + "_out.jpg") << (randomFileName + ".pc");
        QProcess prog(this);
        prog.start(targetEXE, args);
        prog.waitForStarted();
        prog.write("test\n");
        prog.waitForBytesWritten();
        prog.write("test\n");
        prog.waitForBytesWritten();
        prog.waitForReadyRead();
        QByteArray bytes = prog.readAll();
        prog.waitForFinished();
        //QByteArray readData = prog.readAll();
        prog.close();
        // Cleaning - Deleting temp files

    }
    else {

    }

}

/*!
 * \brief ModelPC::circuit The brain of the app. Via special circuit stores data in image.
 * \details The circuit itself can be found in documentation or in commentaries in source.
 * \param image Image to be processed.
 * \param data Data to be processed.
 * \param countBytes Number of bytes to be read or written.
 * \sa ModelPC::processPixel
 */
void ModelPC::circuit(QImage *image, QByteArray *data, long long countBytes)
{

    // Some flags and creation of the ProgressDialog
    success = true;
    emit setProgress(-1);
    bool isEncrypt = !data->isEmpty();

    // Image setup
    int w = image->width();
    int h = image->height();

    // Visited pixels array
    QVector <QPoint> were;
    were.push_back(QPoint(0, 0));
    were.push_back(QPoint(0, h - 1));
    were.push_back(QPoint(w - 1, 0));
    were.push_back(QPoint(w - 1, h - 1));

    long long int offset = 0;

    // Pre-start Cleaning
    circuitData = data;
    circuitImage = image;
    circuitCountBytes = countBytes;
    cur = 0;
    bitsBuffer.clear();

    // Writing Top-Left to Bottom-Left
    for(int i = 1; i < h - 1 && mustGoOn(isEncrypt); i++) {
        QPoint pos(0, i);
        processPixel(pos, &were, isEncrypt);
    }
    // Writing Bottom-Right to Top-Right
    if(mustGoOn(isEncrypt))
    {
        for(int i = h - 2; i >= 1 && mustGoOn(isEncrypt); i--){
            QPoint pos(w - 1, i);
            processPixel(pos, &were, isEncrypt);
        }
    }
    // Main cycle
    // Strong is considered as actual corner pixel and weak as pixel near it like (1, 0) or (0, 1)
    while(mustGoOn(isEncrypt))
    {
        // Strong Top-Right to Strong Bottom-Right
        for(int i = offset; i < h - offset && mustGoOn(isEncrypt); i++){
            QPoint pos(w - offset - 2, i);
            processPixel(pos, &were, isEncrypt);
        }
        // Strong Top-Left to Weak Top-Right
        for(int i = offset + 1; i < w - offset - 2 && mustGoOn(isEncrypt); i++){
            QPoint pos(i, offset);
            processPixel(pos, &were, isEncrypt);
        }
        // Weak Bottom-Right to Weak Bottom-Left
        for(int i = w - 3 - offset; i >= offset + 2 && mustGoOn(isEncrypt); i--){
            QPoint pos(i, h - offset - 1);
            processPixel(pos, &were, isEncrypt);
        }
        // Weak Top-Left to Strong Bottom-Left
        for(int i = offset + 1; i < h - offset && mustGoOn(isEncrypt); i++){
            QPoint pos(offset + 1, i);
            processPixel(pos, &were, isEncrypt);
        }
        offset++;
    }
    // Extra writing
    if(!success)
        return;
    if(isEncrypt)
    {
        // Getting past colors
        QColor colUL = image->pixelColor(0, 0).toRgb();
        QColor colUR = image->pixelColor(w - 1, 0).toRgb();
        QColor colDL = image->pixelColor(0, h - 1).toRgb();
        QColor colDR = image->pixelColor(w - 1, h - 1).toRgb();
        int red = 0;
        int green = 0;
        int blue = 0;

        // Writing Upper Left
        red = (colUL.red() & 224) + (countBytes >> 19);
        green = (colUL.green() & 224) + (countBytes >> 14) % 32;
        blue = (colUL.blue() & 224) + (countBytes >> 9) % 32;
        image->setPixelColor(0, 0, QColor(red, green, blue));

        // Writing Upper Right
        red = (colUR.red() & 224) + (countBytes >> 4) % 32;
        green = (colUR.green() & 224) + ((countBytes % 16) << 1) + 1;
        blue = (colUR.blue() & 224) + 9;
        image->setPixelColor(w - 1, 0, QColor(red, green, blue));

        // Getting extra bytes if left
        while(cur < countBytes)
            push(mod(circuitData->at(cur++)), 8);
        if(bitsBuffer.size() > 20) {
            fail("bitsBufferFail");
            return;
        }
        // Getting extra data as long.
        long extraData = pop(-2);

        // Writing Down Left
        red = (colDL.red() & 224) + (extraData >> 15);
        green = (colDL.green() & 224) + (extraData >> 10) % 32;
        blue = (colDL.blue() & 224) + (extraData >> 5) % 32;
        image->setPixelColor(0, h - 1, QColor(red, green, blue));

        // Writing Down Right
        red = (colDR.red() & 224) + extraData % 32;
        green = (colDR.green() & 224);
        blue = (colDR.blue() & 224) + ((bitsUsed - 1) << 2) + 2;
        image->setPixelColor(w - 1, h - 1, QColor(red, green, blue));
    }
    else
    {
        // Read the past pixels
        QColor colDL = image->pixelColor(0, h - 1).toRgb();
        QColor colDR = image->pixelColor(w - 1, h - 1).toRgb();

        // Read extra data
        long extraData = ((colDL.red() % 32) << 15) + ((colDL.green() % 32) << 10);
        extraData += ((colDL.blue() % 32) << 5) + colDR.red() % 32;

        // Add extra data to the bitsBuffer
        push(extraData, (countBytes - cur) * 8 - bitsBuffer.size());

        // Move bits from bitsBuffer to the QByteArray
        while(!bitsBuffer.isEmpty())
            data->append(pop(8));
    }
    emit setProgress(101);
}

/*!
 * \brief ModelPC::processPixel Processes every pixel. Reads its contains or writes data
 * \param pos Position of pixel
 * \param were Vector array containing pixels, that were already processed.
 * \param isEncrypt Mode of operation.
 * If true encryption operations will continue, else the decryption ones.
 */
void ModelPC::processPixel(QPoint pos, QVector<QPoint> *were, bool isEncrypt)
{
    if(!success)
        return;
    // Check if point was already visited
    if(were->contains(pos)){
        fail("Point (" + QString::number(pos.x()) + "," + QString::number(pos.y()) + ") was visited twice! Error code 2");
        return;
    }
    else
        were->push_back(pos);
    if(isEncrypt)
    {
        // Make sure that there are enough bits in bitsBuffer to write
        while(bitsBuffer.size() < 3 * bitsUsed)
            push(mod(circuitData->at(cur++)), 8);
        // Read past contains
        QColor pixelColor = circuitImage->pixelColor(pos);
        int red = pixelColor.red();
        int green = pixelColor.green();
        int blue = pixelColor.blue();

        // Write new data in last bitsUsed pixels
        red += pop() - red % (int) pow(2, bitsUsed);
        green += pop() - green % (int) pow(2, bitsUsed);
        blue += pop() - blue % (int) pow(2, bitsUsed);

        circuitImage->setPixelColor(pos, QColor(red, green, blue));
    }
    else
    {
        QColor read_color = circuitImage->pixelColor(pos).toRgb();
        // Reading the pixel
        int red = read_color.red();
        int green = read_color.green();
        int blue = read_color.blue();

        // Reading the last bitsUsed pixels
        red %= (int) pow(2, bitsUsed);
        green %= (int) pow(2, bitsUsed);
        blue %= (int) pow(2, bitsUsed);

        // Getting the data in the bitsBuffer.
        push(red);
        push(green);
        push(blue);

        // Getting data to QByteArray
        while(bitsBuffer.size() >= 8) {
            circuitData->append(pop(8));
            cur++;
        }
    }
    emit setProgress(100 * cur / circuitCountBytes);
}

long ModelPC::pop(int bits)
{
    // Hard to say
    long res = 0;
    int poppedBits = bits == -1 ? bitsUsed : bits;
    if(bits == -2)
        poppedBits = bitsBuffer.size();
    for(int i = 0; i < poppedBits; i++)
        res += bitsBuffer[i] * pow(2, poppedBits - i - 1);
    bitsBuffer.remove(0, poppedBits);
    return res;
}

void ModelPC::push(int data, int bits)
{
    // That's easier, but also hard
    int buf_size = bitsBuffer.size();
    int extraSize = bits == -1 ? bitsUsed : bits;
    bitsBuffer.resize(buf_size + extraSize);
    for(int i = bitsBuffer.size() - 1; i >= buf_size; i--, data >>= 1)
        bitsBuffer[i] = data % 2;
}

bool ModelPC::mustGoOn(bool isEncrypt)
{
    return success && (isEncrypt ? (circuitCountBytes - cur) * 8 + bitsBuffer.size() >= bitsUsed * 3 :
                                   circuitData->size() * 8 + bitsBuffer.size() <
                                   circuitCountBytes * 8 - (circuitCountBytes * 8)% (bitsUsed * 3));
}
/*!
 * \brief ModelPC::unzip Unzip data from ModelPC::decrypt.
 * Just mirrored EncryptDialog::zip.
 * \param data Data to be decrypted.
 * \param key Key to decrypt the data.
 * \return Returns data
 * \sa EncryptDialog::zip, ModelPC::decrypt, ModelPC::zip
 */
QByteArray ModelPC::unzip(QByteArray data, QByteArray key)
{
    // Decryption
    QByteArray hashKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::ECB);
    QByteArray new_data = encryption.decode(data, hashKey);
    // Decompressing
    return qUncompress(new_data);
}
/*!
 * \brief ModelPC::zip Zip function, copy of EncryptDialog::zip
 * Used for ModelPC in custom projects, other than PictureCrypt.
 * \param data Data to be encrypted
 * \param key Key for encryption
 * \return Returns decrypted data
 * \sa ModelPC::start, ModelPC::encrypt, ModelPC::unzip
 */
QByteArray ModelPC::zip(QByteArray data, QByteArray key)
{
    // Zip
    QByteArray c_data = qCompress(data, 9);
    // Encryption
    QByteArray hashKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
    return QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::ECB, c_data, hashKey);
}

bool ModelPC::fileExists(QString path)
{
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}

/*!
 * \brief ModelPC::bytes Turns number to QByteArray.
 * (not really in use)
 * \param n The number
 * \return Returns QByteArray
 */
QByteArray ModelPC::bytes(long long n)
{
    return QByteArray::fromHex(QByteArray::number(n, 16));
}
/*!
 * \brief ModelPC::mod As QByteArray is signed so elements over 128 are less than zero.
 * So here we turn them to unsigned ints.
 * \param input Number to be unsigned.
 * \return Returns unsigned.
 */
unsigned int ModelPC::mod(int input)
{
    if(input < 0)
        return (unsigned int) (256 + input);
    else
        return (unsigned int) input;
}
/*!
 * \brief ModelPC::alert Function emits signal ModelPC::alertView and calls ViewPC::alert.
 * \param message Message to be transmitted.
 * \param isWarning Flag if message is critical.
 * \sa ViewPC::alert
 */
void ModelPC::alert(QString message, bool isWarning)
{
    emit alertView(message, isWarning);
}
/*!
 * \brief ModelPC::RGBbytes Turns bytes into QColor by RGB.
 * \param byte Data to be processed.
 * \return Returns color.
 */
QColor ModelPC::RGBbytes(long long byte)
{
    int blue = byte % 256;
    int green = (byte / 256) % 256;
    int red = byte / pow(2, 16);
    return QColor(red, green, blue);
}

QString ModelPC::generateVersionString(long ver)
{
    return QString::number((int)( ver / pow(2, 16))) + "." + QString::number(((int) (ver / 256)) % 256) + "." + QString::number(ver % 256);
}

uint ModelPC::randSeed()
{
    QTime time = QTime::currentTime();
    uint randSeed = time.msecsSinceStartOfDay() % 65536 + time.minute() * 21 + time.second() * 2;
    return randSeed;
}
