#ifndef VIEWPC_H
#define VIEWPC_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QByteArray>
#include <QVector>
#include <QThread>
#include <QDesktopServices>

#include <encryptdialog.h>
#include <QProgressDialog>
#include <aboutpc.h>

#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class ViewPC;
}
/*! \file viewpc.h
 * Header of ViewPC class
 * \sa ControllerPC, ModelPC, ViewPC
*/
/*!
 * \brief The ViewPC class View layer of the app.
 * Controls EncryptDialog and ProgressDialog.
 * \sa ControllerPC, ModelPC, EncryptDialog
 */
class ViewPC : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewPC(QWidget *parent = nullptr);
    ~ViewPC();
private slots:
    void on_encryptMode_clicked();

    void on_decryptMode_clicked();

    void on_actionJPHS_path_triggered();

    void on_actionRun_tests_triggered();

protected slots:
    void on_fileButton_clicked();

    void on_startButton_clicked();

    void on_actionAbout_triggered();

    void on_actionHelp_triggered();
public slots:
    void alert(QString message, bool isWarning = false);
    void saveData(QByteArray Edata);
    void saveImage(QImage *image);
    void setProgress(int val);
    void abortCircuit();
    void setEncryptMode(bool encr);
    void setVersion(QString version);
signals:
    /*!
     * \brief encrypt Signal calling ModelPC::encrypt
     * \param data Data to write
     * \param image Image to be encrypted into.
     * \param mode Mode of encryption
     */
    encrypt(QByteArray data, QImage * image, int mode, int bitsUsed);
    /*!
     * \brief decrypt Signal calling ModelPC::decrypt
     * \param _image Image for decryption
     */
    decrypt(QImage * _image);
    /*!
     * \brief abortModel Signal calling to stop ModelPC::circuit
     */
    abortModel();
    /*!
     * \brief setJPHSPath Sets the default JPHS directory
     * \param dir Directory
     */
    setJPHSDir(QString dir);
public:
    /*!
     * \brief dialog ProgressDialog used.
     * \sa ViewPC::setProgress, ViewPC::cancel, ModelPC::setProgress
     */
    QProgressDialog * dialog;
    /*!
     * \brief progressDialogClosed Flag, if dialog is closed.
     * \sa ViewPC::abortCircuit, ViewPC::setProgress
     */
    bool progressDialogClosed;
    QJsonObject errorsDict;
private:
    Ui::ViewPC *ui;
    bool isEncrypt;
    QString inputFileName;
    QByteArray bytes(long long n);
    QString versionString;
};

#endif // VIEWPC_H
