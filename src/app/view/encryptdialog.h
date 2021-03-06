﻿#ifndef ENCRYPTDIALOG_H
#define ENCRYPTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QString>

#include "../model/qaesencryption.h"
#include <QCryptographicHash>

namespace Ui {
class EncryptDialog;
}
/*!
 * \brief The EncryptDialog class Class to get the image and key to store secret info.
 * \note Not the most important and well written class.
 * \sa ViewPC
 */
class EncryptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncryptDialog(QByteArray _data, QWidget *parent = 0);
    ~EncryptDialog();

public slots:
    void on_fileButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_bitsSlider_valueChanged(int value);

public:
    /*!
     * \brief data Input data
     */
    QByteArray data;
    /*!
     * \brief success Flag, if image was successfully selected and data was encrypted.
     */
    bool success;
    /*!
     * \brief compr_data Compressed data, aka Output data.
     */
    QByteArray compr_data;
    /*!
     * \brief inputFileName Filename of the image.
     */
    QString inputFileName;
    /*!
     * \brief size Size of the image in square pixels
     */
    long long int size;
    /*!
     * \brief key Key to be used for encryption in EncrytDialog::zip
     */
    QString key;
    /*!
     * \brief goodPercentage Flag if area of the used data via encryption is less than 70% of the area of the image.
     */
    bool goodPercentage;
    /*!
     * \brief val Value of the slider
     */
    int val;
    /*!
     * \brief bitsUsed Bits used per byte of pixel.
     * \sa ModelPC::circuit
     */
    int bitsUsed;
    /*!
     * \brief image Inputted image
     */
    QImage image;
    QByteArray zip();
private:
    Ui::EncryptDialog *ui;
    void alert(QString text);
};

#endif // ENCRYPTDIALOG_H
