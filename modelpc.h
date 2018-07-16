#ifndef MODELPC_H
#define MODELPC_H

#include <QObject>
#include <QFile>
#include <QImage>
#include <algorithm>
#include <QByteArray>
#include <QColor>
#include <QPoint>
#include <QVector>
#include <QThread>
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
    ModelPC(long _version);

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
    void encrypt(QByteArray encr_data, QString imagePath);
    void decrypt(QString inputFileName);
public:
    /*!
     * \brief success Flag that true by default,
     * but in case of error or cancelling of ProgressDialog it turns to false, which stops execution of ModelPC::circuit
     */
    bool success;
    /*!
     * \brief version Version of the app
     */
    long version;
    QByteArray unzip(QByteArray data, long long key);
    void alert(QString message, bool isWarning = false);
protected:
    void circuit(QImage * image, QByteArray * data, long long int countBytes);
    void processPixel(QPoint pos, QVector<QPoint> *were, long long cur, bool isEncrypt);
private:
    QByteArray bytes(long long n);
    unsigned int mod(int input);
    QByteArray ver_byte;
    QColor RGBbytes(long long byte);
    QByteArray * circuitData;
    QImage * circuitImage;
    long long circuitCountBytes;
    QString generateVersionString(long ver);
};

#endif // MODELPC_H
