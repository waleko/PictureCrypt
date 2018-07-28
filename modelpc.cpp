#include "modelpc.h"
/*!
 * \brief ModelPC::ModelPC Constructor
 * \param _version Version of the app from Controller.
 * \sa ControllerPC, ViewPC
 */
ModelPC::ModelPC(long _version)
{
    // Version control
    version = _version;
    ver_byte = bytes((long int) (version / 65536)) + bytes((long int) (version / 256) % 256);
    ver_byte += bytes(version % 256);
}
/*!
 * \brief ModelPC::encrypt Slot to be called when encrypt mode in ViewPC is selected and started.
 * \param encr_data Data to be inserted to an image.
 * \param imagePath Filename of image to be inserted in.
 * \param mode Mode of encryption
 * \sa ViewPC::on_startButton_clicked, ModelPC::decrypt, ModelPC::circuit
 */
void ModelPC::encrypt(QByteArray encr_data, QString imagePath, int mode)
{
    // Image
    QImage * image = new QImage(imagePath);
    encr_data = ver_byte + encr_data;

    long long int countBytes = encr_data.size();
    circuit(image, &encr_data, countBytes);

    // Saving
    if(success)
        emit saveImage(image);
}
/*!
 * \brief ModelPC::decrypt Slot to be called when decrypt mode in ViewPC is selected and started.
 * \param inputFileName Filename of image to be decrypted.
 * \sa ViewPC::on_startButton_clicked, ModelPC::encrypt, ModelPC::circuit
 */
void ModelPC::decrypt(QString inputFileName)
{
    // Image opening
    QImage * image = new QImage(inputFileName);
    int w = image->width();
    int h = image->height();

    // Checking if file has flags of being crypted by this app
    QColor colUL = image->pixelColor(0, 0).toRgb();
    QColor colUR = image->pixelColor(w - 1, 0).toRgb();
    QColor colDR = image->pixelColor(w - 1, h - 1).toRgb();

    int verifCode = (((colUR.green() % 2) << 5) + colUR.blue() % 32) << 2;
    verifCode += colDR.blue() % 4;
    if(verifCode != 166){
        alert("Image wasn't encrypted by this app or is damaged!");
        return;
    }

    long long int countBytes = (colUL.blue() % 32 + ((colUL.green() % 32) << 5) + ((colUL.red() % 32) << 10)) << 9;
    countBytes += ((colUR.red() % 32) << 4) + (colUR.green() >> 1) % 16;

    bitsUsed = (colDR.blue() >> 2) % 8 + 1;
    int mode = colDR.green() % 32;

    // Start of the circuit
    QByteArray data;
    circuit(image, &data, countBytes);

    if(!success)
        return;
    if(data.isEmpty())
    {
        alert("Read data is empty. Cannot continue!", true);
        return;
    }
    // Version check
    long long int _ver = mod(data.at(0) * pow(2, 16));
    _ver += mod(data.at(1) * pow(2, 8));
    _ver += mod(data.at(2));
    data.remove(0, 3);
    if(_ver > version) {
        alert("Picture's app version is newer than yours. Image version is "
              + generateVersionString(_ver) + ", yours is "
              + generateVersionString(version) + ".", true);
        return;
    }
    else if(_ver < version) {
        alert("Picture's app version is older than yours. Image version is "
              + generateVersionString(_ver) + ", yours is "
              + generateVersionString(version) + ".", true);
        return;
    }
    // Obtain the key
    long long int key = mod(data.at(0) * pow(2, 16));
    key += mod(data.at(1) * pow(2, 8));
    key += mod(data.at(2));
    data.remove(0, 3);
    // Unzip
    QByteArray unzipped_data = unzip(data, key);
    emit saveData(unzipped_data);
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
        QColor colUL = image->pixelColor(0, 0).toRgb();
        QColor colUR = image->pixelColor(w - 1, 0).toRgb();
        QColor colDL = image->pixelColor(0, h - 1).toRgb();
        QColor colDR = image->pixelColor(w - 1, h - 1).toRgb();
        int red = 0;
        int green = 0;
        int blue = 0;

        red = (colUL.red() & 224) + (countBytes >> 19);
        green = (colUL.green() & 224) + (countBytes >> 14) % 32;
        blue = (colUL.blue() & 224) + (countBytes >> 9) % 32;
        image->setPixelColor(0, 0, QColor(red, green, blue));

        red = (colUR.red() & 224) + (countBytes >> 4) % 32;
        green = (colUR.green() & 224) + ((countBytes % 16) << 1) + 1;
        blue = (colUR.blue() & 224) + 9;
        image->setPixelColor(w - 1, 0, QColor(red, green, blue));

        while(cur < countBytes)
            push(mod(circuitData->at(cur++)), 8);
        if(bitsBuffer.size() > 20) {
            alert("Something went wrong! Error code 1", true);
            success = false;
            emit setProgress(101);
            return;
        }

        long extraData = pop(-2);

        red = (colDL.red() & 224) + (extraData >> 15);
        green = (colDL.green() & 224) + (extraData >> 10) % 32;
        blue = (colDL.blue() & 224) + (extraData >> 5) % 32;
        image->setPixelColor(0, h - 1, QColor(red, green, blue));

        red = (colDR.red() & 224) + extraData % 32;
        green = (colDR.green() & 224);
        blue = (colDR.blue() & 224) + ((bitsUsed - 1) << 2) + 2;
        image->setPixelColor(w - 1, h - 1, QColor(red, green, blue));
    }
    else
    {
        QColor colDL = image->pixelColor(0, h - 1).toRgb();
        QColor colDR = image->pixelColor(w - 1, h - 1).toRgb();
        long extraData = ((colDL.red() % 32) << 15) + ((colDL.green() % 32) << 10);
        extraData += ((colDL.blue() % 32) << 5) + colDR.red() % 32;
        push(extraData, (countBytes - cur) * 8 - bitsBuffer.size());

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
    // Check if point was visited
    if(were->contains(pos)){
        alert("Point (" + QString::number(pos.x()) + "," + QString::number(pos.y()) + ") was visited twice! Error code 2", true);
        success = false;
        emit setProgress(101);
        return;
    }
    else
        were->push_back(pos);
    if(isEncrypt)
    {
        while(bitsBuffer.size() < 3 * bitsUsed)
            push(mod(circuitData->at(cur++)), 8);
        QColor pixelColor = circuitImage->pixelColor(pos);
        int red = pixelColor.red();
        int green = pixelColor.green();
        int blue = pixelColor.blue();

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

        red %= (int) pow(2, bitsUsed);
        green %= (int) pow(2, bitsUsed);
        blue %= (int) pow(2, bitsUsed);

        push(red);
        push(green);
        push(blue);

        while(bitsBuffer.size() >= 8) {
            circuitData->append(pop(8));
            cur++;
        }
    }
    emit setProgress(100 * cur / circuitCountBytes);
}

long ModelPC::pop(int bits)
{
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
 * \sa EncryptDialog::zip, ModelPC::decrypt
 */
QByteArray ModelPC::unzip(QByteArray data, long long key)
{
    // Decryption
    QByteArray new_data;
    bool keyB = key;
    if(!keyB)
        key = data.size();
    QByteArray bkey = bytes(key);
    for(long long int i = 0; i < data.size(); i++)
        new_data.append(data.at(i) ^ bkey.at(i % bkey.size()));
    // Decompressing
    if(keyB)
        return qUncompress(new_data);
    else
        return new_data;
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
