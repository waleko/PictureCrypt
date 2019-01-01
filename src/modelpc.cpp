#include "modelpc.h"
#include <QDebug>
#include <QtMath>
/*!
 * \brief ModelPC::ModelPC Constructor
 * Unit tests are run here.
 * \sa ControllerPC, ViewPC
 */
ModelPC::ModelPC()
{
    // Version control
    versionString = "1.3.7";

    auto ver = versionString.split(".");
    version = ver[0].toInt() * qPow(2, 16) + ver[1].toInt() * qPow(2, 8) + ver[2].toInt();

    ver_byte = bytes(ver[0].toInt()) +
            bytes(ver[1].toInt()) +
            bytes(ver[2].toInt());
    // Random seed
    qsrand(randSeed());
}

QImage *ModelPC::Encrypt(QByteArray data, QImage *image, int _mode, QString key, int _bitsUsed, QString *_error)
{
    return ModelPC().encrypt(data, image, _mode, key, _bitsUsed, _error);
}

QImage *ModelPC::Inject(QByteArray encr_data, QImage *image, int _mode, int _bitsUsed, QString *_error)
{
    return ModelPC().inject(encr_data, image, _mode, _bitsUsed, _error);
}

QByteArray ModelPC::Decrypt(QImage *image, QString key, int _mode, QString *_error)
{
    return ModelPC().decrypt(image, key, _mode, _error);
}
/*!
 * \brief ModelPC::encrypt Slot to zip and inject data and provide it with some extra stuff
 * After completion start standard ModelPC::inject
 * Isn't used in PictureCrypt, but used can be used in other - custom projects.
 * \param data Data for embedding
 * \param image Image for embedding
 * \param mode Mode for embedding
 * \param key Key for extra encryption
 * \param _bitsUsed Bits per byte (see ModelPC::bitsUsed)
 * \param _error Error output
 * \return Returns image with embedded data
 * \sa ModelPC::inject
 */
QImage * ModelPC::encrypt(QByteArray data, QImage * image, int _mode, QString key, int _bitsUsed, QString *_error)
{
    // FIXME check for errors
    CryptMode mode = CryptMode(_mode);
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
        fail("no_key");
        return nullptr;
    }
    else if(key.size() > 255) {
        fail("bigkey");
        return nullptr;
    }
    if(mode == CryptMode::NotDefined) {
        fail("undefined_mode");
        return nullptr;
    }
    long long usedBytes = data.size() + 14 + key.size();
    long long size = image->width() * image->height();
    if(usedBytes * 100 / (size * 3) * 8 / _bitsUsed > 70) {
        fail("muchdata");
        return nullptr;
    }

    switch(mode)
    {
        case v1_3:
        {
            QByteArray zipped_data = zip(data, key.toUtf8());
            QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
            QByteArray encr_data = hash + zipped_data;
            if(*error == "ok")
                return inject(encr_data, image, _mode, _bitsUsed, error);
            else
                return nullptr;
            break;
        }
        case v1_4:
            bitsUsed = _bitsUsed;
            encryptv1_4(image, data, key);
            return image;
        break;
        case jphs_mode:
            // TODO add jphs
            return nullptr;
        break;
        default:
            fail("wrongmode");
            return nullptr;
    }
}

/*!
 * \brief ModelPC::inject Slot to be called when encrypt mode in ViewPC is selected and started.
 * \param encr_data Data to be inserted to an image.
 * \param image Image to be inserted in.
 * \param mode Mode of encryption
 * \param _bitsUsed Bits per byte used
 * \param _error Error output
 * \return Returns image with embedded data.
 * \sa ViewPC::on_startButton_clicked, ModelPC::decrypt, ModelPC::circuit, ModelPC::start
 */
QImage * ModelPC::inject(QByteArray encr_data, QImage * image, int _mode, int _bitsUsed, QString *_error)
{
    CryptMode mode = CryptMode(_mode);
    // Error management
    if(_error == nullptr)
        _error = new QString();
    *_error = "ok";
    error = _error;

    bitsUsed = _bitsUsed;

    if(encr_data.isEmpty()) {
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
    if(mode == CryptMode::NotDefined) {
        fail("undefined_mode");
        return nullptr;
    }

    encr_data = ver_byte + encr_data;
    long long int countBytes = encr_data.size();
    switch(mode)
    {
    case v1_3:
        circuit(image, &encr_data, countBytes);
        break;
    case jphs_mode:
        jphs(image, &encr_data);
        break;
    case v1_4:
        fail("inject-v1.4");
        return nullptr;
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
 * \param key Keyphrase with which the data is injected
 * \param _mode Mode for decryption
 * \param _error Error output
 * \return Returns decrypted data
 * \sa ViewPC::on_startButton_clicked, ModelPC::inject, ModelPC::circuit
 */
QByteArray ModelPC::decrypt(QImage * image, QString key, int _mode, QString *_error)
{
    CryptMode mode = CryptMode(_mode);
    // Error management
    if(_error == nullptr)
        _error = new QString();
    *_error = "ok";
    error = _error;
    if(image == nullptr || image->isNull()) {
        fail("nullimage");
        return nullptr;
    }
    QByteArray result;

    switch (mode) {
    case v1_3:
        result = decryptv1_3(image, key);
    break;
    case v1_4:
        result = decryptv1_4(image, key);
    break;
    case jphs_mode:
        // TODO add jphs support
    break;
    case NotDefined:
        // TODO check all upper functions
    break;
    default:
        // For invalid modes
        fail("wrongmode");
        return nullptr;
    }
    if(*error == "ok")
        emit saveData(result);
    return result;
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
        red += pop() - red % (int) qPow(2, bitsUsed);
        green += pop() - green % (int) qPow(2, bitsUsed);
        blue += pop() - blue % (int) qPow(2, bitsUsed);

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
        red %= (int) qPow(2, bitsUsed);
        green %= (int) qPow(2, bitsUsed);
        blue %= (int) qPow(2, bitsUsed);

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
/*!
 * \brief ModelPC::encryptv1_4 Encrypts and injects data to image used in v1.4+
 * \param image Image for injecting
 * \param data Data for embedding
 */
void ModelPC::encryptv1_4(QImage *image, QByteArray data, QString key)
{
    QByteArray rand_master = GetRandomBytes(32);
    QByteArray pass = QCryptographicHash::hash(key.toUtf8() + rand_master + QString("hi").toUtf8(), QCryptographicHash::Sha3_384);
    QByteArray noise = GetRandomBytes(data.size() / 10 + 32);
    QByteArray bytes_key = GetRandomBytes(32);
    QByteArray pass_rand = QCryptographicHash::hash(pass + bytes_key, QCryptographicHash::Sha3_256);
    QByteArray zipped = zip(data, pass_rand);
}
/*!
 * \brief ModelPC::decryptv1_4 Decrypts data from image in v1.4+
 * \param image Image with data
 * \param key Key
 * \return Returns obtained data
 */
QByteArray ModelPC::decryptv1_4(QImage *image, QString key)
{

}

/*!
 * \brief ModelPC::decryptv1_3 Decrytps data from image in v1.3
 * \param image Image with data
 * \param key Key
 * \return Returns obtained data
 */
QByteArray ModelPC::decryptv1_3(QImage *image, QString key)
{
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
    // FIXME check if works
    // curMode = colDR.green() % 32;

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
    long long int _ver = mod(data.at(0)) * qPow(2, 16);
    _ver += mod(data.at(1)) * qPow(2, 8);
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
    // Get the hash
    QByteArray hash = data.left(32);
    data.remove(0, 32);

    // Unzip
    QByteArray unzipped_data = unzip(data, key.toUtf8());
    QByteArray our_hash = QCryptographicHash::hash(unzipped_data, QCryptographicHash::Sha256);
    if(our_hash != hash) {
        fail("fail_hash");
        return QByteArray("");
    }
    return unzipped_data;
}
long ModelPC::pop(int bits)
{
    // Hard to say
    long res = 0;
    int poppedBits = bits == -1 ? bitsUsed : bits;
    if(bits == -2)
        poppedBits = bitsBuffer.size();
    for(int i = 0; i < poppedBits; i++)
        res += bitsBuffer[i] * qPow(2, poppedBits - i - 1);
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
 * \sa ModelPC::start, ModelPC::inject, ModelPC::unzip
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
    int red = byte / qPow(2, 16);
    return QColor(red, green, blue);
}

QString ModelPC::generateVersionString(long ver)
{
    return QString::number((int)( ver / qPow(2, 16))) + "." + QString::number(((int) (ver / 256)) % 256) + "." + QString::number(ver % 256);
}

uint ModelPC::randSeed()
{
    QTime time = QTime::currentTime();
    uint randSeed = time.msecsSinceStartOfDay() % 65536 + time.minute() * 21 + time.second() * 2;
    return randSeed;
}
QByteArray ModelPC::GetRandomBytes(long long count)
{
    QByteArray res;
    for(int i = 0; i < count; i++)
       res += qrand() % 256;
    return res;
}
