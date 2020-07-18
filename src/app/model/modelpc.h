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
#include <QtGui>
#include <QtCore/QRandomGenerator>
#include <QPair>

#include "qaesencryption.h"
#include <QCryptographicHash>


/*! \file modelpc.h
 * Header of ModelPC class
 * \sa ControllerPC, ModelPC, ViewPC
*/
/*!
 * \brief The ModelPC class
 * Model Layer of the app.
 * Main class that does the work of PictureCrypt logic
 * Controled by ControllerPC
 * \sa ViewPC, ControllerPC
 * \author Alex Kovrigin (waleko)
 */
class ModelPC : public QObject
{
    Q_OBJECT
public:
    ModelPC();
    enum CryptMode {NotDefined, v1_3, v1_4, jphs_mode};
    static QImage *Encrypt(QByteArray data, QImage *image, CryptMode _mode, QString key = "", int _bitsUsed = 8, QString *_error = nullptr);
    static QByteArray Decrypt(QImage * image, QString key, CryptMode _mode = NotDefined, QString *_error = nullptr);

signals:
    /*!
     * \brief alertView Signal to be called to create MessageBox.
     * \param messageCode Message Code to be shown.
     * \param isWarning Flag if message is critical.
     * \sa ModelPC::alert, ViewPC::alert
     */
    void alertView(QString messageCode, bool isWarning);
    /*!
     * \brief saveData Signal to be called to save data from ModelPC::decrypt.
     * \param data Data to be saved.
     */
    void saveData(QByteArray data);
    /*!
     * \brief saveImage Signal to be called to save image from ModelPC::encrypt.
     * \param image Image to be saved.
     */
    void saveImage(QImage *image);
    /*!
     * \brief setProgress Signal to be called to set progress of ProgressDialog.
     * \param val Value to be set.
     * \sa ViewPC::setProgress
     */
    void setProgress(int val);

public slots:
    QImage *encrypt(QByteArray data, QImage *image, CryptMode _mode, QString key = "", int _bitsUsed = 8, QString *_error = nullptr);
    QByteArray decrypt(QImage * image, QString key, CryptMode _mode = NotDefined, QString *_error = nullptr);
    void fail(QString message);
    void alert(QString message, bool isWarning = false);

public:
    QByteArray unzip(QByteArray data, QByteArray key);

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
     * \brief defaultJPHSDir Default JPHS directory
     */
    QString defaultJPHSDir;
protected:
    static QImage *Inject(QByteArray encr_data, QImage * image, CryptMode _mode, int _bitsUsed = 8, QString *_error = nullptr);

    void circuit(QImage * image, QByteArray * data, long long int countBytes);
    void jphs(QImage * image, QByteArray * data);
    void processPixel(QPoint pos, QVector<QPoint> *were, bool isEncrypt);
    void encryptv1_4(QImage *image, QByteArray data, QString key);
    QByteArray decryptv1_3(QImage * image, QString key);
    QByteArray decryptv1_4(QImage * image, QString key);
    void proccessPixelsv1_4(QImage *image, QByteArray* data, QByteArray key, bool isEncrypt, QVector<QPair<QPoint, QPair<int, int> > > *were, long long size = -1);
    QByteArray zip(QByteArray data, QByteArray key);

    /*!
     * \brief error Current error
     */
    QString * error;
private:
    int bitsUsed;
    bool fileExists(QString path);
    QByteArray bytes(long long n);
    unsigned int mod(int input);
    QByteArray ver_byte;
    QColor RGBbytes(long long byte);
    QString generateVersionString(long ver);
    uint randSeed();
    bool isTry = false;

    QByteArray * circuitData;
    QImage * circuitImage;
    long long circuitCountBytes;
    long cur;
    bool mustGoOn(bool isEncrypt);

    QVector <bool> bitsBuffer;
    long pop(int bits = -1);
    void push(int data, int bits = -1);

    void setError(QString word);
    QByteArray GetRandomBytes(long long count = 32);
protected slots:
    QImage *inject(QByteArray encr_data, QImage * image, CryptMode _mode, int _bitsUsed = 8, QString *_error = nullptr);
};

#endif // MODELPC_H
