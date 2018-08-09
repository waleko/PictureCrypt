#ifndef MODELPC_H
#define MODELPC_H

#include <QObject>
#include <QImage>
#include <QByteArray>
#include <QColor>
#include <QPoint>
#include <QVector>
#include <QProcess>
#include <QTime>
#include <QFileInfo>

#include <aes/qaesencryption.h>
#include <QCryptographicHash>

/*! \file modelpc.h
 * Header of ModelPC class
 * \sa ControllerPC, ModelPC, ViewPC
*/
/*!
 * \brief The ModelPC class
 * Model Layer of the app.
 * Controled by ControllerPC
 * \sa ViewPC, ControllerPC
 */
class ModelPC : public QObject
{
    Q_OBJECT
public:
    ModelPC();

signals:
    /*!
     * \brief alertView Signal to be called to create MessageBox.
     * \param message Message to be shown.
     * \param isWarning Flag if message is critical.
     * \sa ModelPC::alert, ViewPC::alert
     */
    alertView(QString message, bool isWarning);
    /*!
     * \brief saveData Signal to be called to save data from ModelPC::decrypt.
     * \param data Data to be saved.
     */
    saveData(QByteArray data);
    /*!
     * \brief saveImage Signal to be called to save image from ModelPC::encrypt.
     * \param image Image to be saved.
     */
    saveImage(QImage *image);
    /*!
     * \brief setProgress Signal to be called to set progress of ProgressDialog.
     * \param val Value to be set.
     * \sa ViewPC::setProgress
     */
    setProgress(int val);
public slots:
    void start(QByteArray data, QImage image, int mode = 0, QString key = "", int _bitsUsed = 8);
    void encrypt(QByteArray encr_data, QImage * image, int mode = 0);
    void decrypt(QImage * image);
    void fail(QString message);

public:
    /*!
     * \brief success Flag that true by default,
     * but in case of error or cancelling of ProgressDialog it turns to false, which stops execution of ModelPC::circuit
     */
    bool success;
    /*!
     * \brief version Version of the class
     */
    long version;
    /*!
     * \brief versionString Version as string
     */
    QString versionString;
    /*!
     * \brief curMode Mode of en- or decryption
     */
    int curMode;
    /*!
     * \brief bitsUsed Bits per byte used in pixel
     */
    int bitsUsed;
    /*!
     * \brief defaultJPHSDir Default JPHS directory
     */
    QString defaultJPHSDir;
    QByteArray unzip(QByteArray data, QByteArray key);
    void alert(QString message, bool isWarning = false);
protected:
    void circuit(QImage * image, QByteArray * data, long long int countBytes);
    void jphs(QImage * image, QByteArray * data);
    void processPixel(QPoint pos, QVector<QPoint> *were, bool isEncrypt);
    QByteArray zip(QByteArray data, QByteArray key);
private:
    bool fileExists(QString path);
    QByteArray bytes(long long n);
    unsigned int mod(int input);
    QByteArray ver_byte;
    QColor RGBbytes(long long byte);
    QString generateVersionString(long ver);
    uint randSeed();

    QByteArray * circuitData;
    QImage * circuitImage;
    long long circuitCountBytes;
    long cur;
    bool mustGoOn(bool isEncrypt);

    QVector <bool> bitsBuffer;
    long pop(int bits = -1);
    void push(int data, int bits = -1);

};

#endif // MODELPC_H
