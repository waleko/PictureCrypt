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
 * \sa ViewPC::on_startButton_clicked, ModelPC::decrypt, ModelPC::circuit
 */
void ModelPC::encrypt(QByteArray encr_data, QString imagePath)
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
    QColor col1 = image->pixelColor(0, 0).toRgb();
    QColor col2 = image->pixelColor(0, h - 1).toRgb();
    bool leftBottom = col2 != QColor(122, 122, 122).toRgb();
    bool leftUpper = col1 != QColor(123, 123, 123).toRgb();
    if(leftBottom || leftUpper) {
        alert("File wasn't encrypted by our app or is damaged. Cannot continue!", true);
        return;
    }
    // Getting other corner pixels and getting number of bytes to read
    QColor count_256_1 = image->pixelColor(w - 1, 0).toRgb();
    QColor count_256_2 = image->pixelColor(w - 1, h - 1).toRgb();
    long long int countBytes = count_256_2.red();
    countBytes += count_256_1.red() * pow(2, 24) + count_256_1.green() * pow(2, 16) + count_256_1.blue() * pow(2, 8);
    QByteArray data;
    // Start of the circuit
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
    image->setPixelColor(0, h - 1, QColor(122, 122, 122));
    image->setPixelColor(0, 0, QColor(123, 123, 123));
    // Corner pixel for extra writing and reading
    QColor count_256_2 = image->pixelColor(w - 1, h - 1).toRgb();
    // Visited pixels array
    QVector <QPoint> were;
    were.push_back(QPoint(0, 0));
    were.push_back(QPoint(0, h - 1));
    were.push_back(QPoint(w - 1, 0));
    were.push_back(QPoint(w - 1, h - 1));

    long long int offset = 0;
    long long int cur = 0;

    circuitData = data;
    circuitImage = image;
    circuitCountBytes = countBytes;

    // Writing Top-Left to Bottom-Left
    for(int i = 1; i < qMin((long long) h - 1, (long long) countBytes / 3 + 1); i++) {
        QPoint pos(0, i);
        processPixel(pos, &were, cur, isEncrypt);
        cur += 3;
    }
    countBytes -= qAbs((h - 2) * 3);
    // Writing Bottom-Right to Top-Right
    if(countBytes >= 3)
    {
        for(int i = h - 2; i >= qMax((long long) 1, h - (long long) (countBytes / 3) - 1); i--){
            QPoint pos(w - 1, i);
            processPixel(pos, &were, cur, isEncrypt);
            cur += 3;
        }
        countBytes -= qAbs((h - 2) * 3);
    }
    // Main cycle
    // Strong is considered as actual corner pixel and weak as pixel near it like (1, 0) or (0, 1)
    while(countBytes >= 3 && success)
    {
        // Strong Top-Right to Strong Bottom-Right
        for(int i = offset; i < qMin(h - offset, (long long) (countBytes / 3) + offset); i++){
            QPoint pos(w - offset - 2, i);
            processPixel(pos, &were, cur, isEncrypt);
            cur += 3;
        }
        countBytes -= qAbs((h - offset * 2) * 3);
        if(countBytes < 3)
            break;
        // Strong Top-Left to Weak Top-Right
        for(int i = offset + 1; i < qMin(w - offset - 2, (long long) (countBytes / 3) + offset + 1); i++){
            QPoint pos(i, offset);
            processPixel(pos, &were, cur, isEncrypt);
            cur += 3;
        }
        countBytes -= qAbs((w - offset * 2 - 3) * 3);
        if(countBytes < 3)
            break;
        // Weak Bottom-Right to Weak Bottom-Left
        for(int i = w - 3 - offset; i >= qMax(offset + 2, w - 2 - offset - (long long) (countBytes / 3)); i--){
            QPoint pos(i, h - offset - 1);
            processPixel(pos, &were, cur, isEncrypt);
            cur += 3;
        }
        countBytes -= qAbs((w - offset * 2 - 4) * 3);
        if(countBytes < 3)
            break;
        // Weak Top-Left to Strong Bottom-Left
        for(int i = offset + 1; i < qMin(h - offset, (long long) (countBytes / 3) + offset + 1); i++){
            QPoint pos(offset + 1, i);
            processPixel(pos, &were, cur, isEncrypt);
            cur += 3;
        }
        countBytes -= qAbs((h - offset * 2 - 1) * 3);
        offset++;
    }
    // Extra writing
    if(!success)
        return;
    if(isEncrypt)
    {
        image->setPixelColor(w - 1, 0, RGBbytes(circuitCountBytes / 256));
        int red = circuitCountBytes % 256;
        int green = 0;
        int blue = 0;
        switch(circuitCountBytes % 3)
        {
        case 0:
            break;
        case 1:
            blue = mod(data->at(cur++));
            break;
        case 2:
            green = mod(data->at(cur++));
            blue = mod(data->at(cur++));
            break;
        }
        QColor col = QColor(red, green, blue);
        image->setPixelColor(w - 1, h - 1, col);
    }
    else
    {
        switch(circuitCountBytes % 3)
        {
        case 0:
            break;
        case 1:
            data->append(mod(count_256_2.blue()));
            break;
        case 2:
            data->append(mod(count_256_2.green()));
            data->append(mod(count_256_2.blue()));
            break;
        }
    }
    emit setProgress(101);
}
/*!
 * \brief ModelPC::processPixel Processes every pixel. Reads its contains or writes data
 * \param pos Position of pixel
 * \param were Vector array containing pixels, that were already processed.
 * It's for debugging and checking purpuse only.
 * \param cur Current byte to be read
 * \param isEncrypt Mode of operation.
 * If true encryption operations will continue, else the decryption ones.
 */
void ModelPC::processPixel(QPoint pos, QVector<QPoint> *were, long long cur, bool isEncrypt)
{
    if(!success)
        return;
    // Check if point was visited
    if(were->contains(pos)){
        alert("Point (" + QString::number(pos.x()) + "," + QString::number(pos.y()) + ") was visited twice!", true);
        alert("This is an extremely critical error, please contact creator of the app via email: a.kovrigin0@gmail.com", true);
        success = false;
        return;
    }
    else
        were->push_back(pos);
    if(isEncrypt)
    {
        // Check if there is enough data to read
        if(circuitData->size() < cur + 3) {
            success = false;
            alert("Data was read uncorrectly!", true);
            alert("This is an extremely critical error, please contact creator of the app via email: a.kovrigin0@gmail.com", true);
            return;
        }
        // Creation of pixel
        int red = mod(circuitData->at(cur++));
        int green = mod(circuitData->at(cur++));
        int blue = mod(circuitData->at(cur++));
        // Saving the pixel
        QColor pixel(red, green, blue);
        circuitImage->setPixelColor(pos, pixel);
    }
    else
    {
        QColor read_color = circuitImage->pixelColor(pos).toRgb();
        // Reading the pixel
        int red = read_color.red();
        int green = read_color.green();
        int blue = read_color.blue();
        // Saving the data
        circuitData->append(red);
        circuitData->append(green);
        circuitData->append(blue);
    }
    emit setProgress(100 * cur / circuitCountBytes);
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

