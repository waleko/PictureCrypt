#include "encryptdialog.h"
#include "ui_encryptdialog.h"
/*!
 * \brief EncryptDialog::EncryptDialog Constructor of the class.
 * Input data is saved here and some variables are set here.
 * \param _data Input data.
 * \param parent Parent (not in use)
 */
EncryptDialog::EncryptDialog(QByteArray _data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncryptDialog)
{
    ui->setupUi(this);
    data = _data;
    success = false;
    // UI setup
    ui->totalBytes->setText(QString::number(data.size()));
    key = qrand() % 256;
    compr_data = zip();
    long long int compr_data_size = compr_data.size();
    ui->zippedBytes->setText(QString::number(compr_data_size));
    goodPercentage = false;
    bitsUsed = 8;
}

EncryptDialog::~EncryptDialog()
{
    delete ui;
}

void EncryptDialog::alert(QString text)
{
    QMessageBox t;
    t.setWindowTitle("Message");
    t.setIcon(QMessageBox::Warning);
    t.setWindowIcon(QIcon(":/mail.png"));
    t.setText(text);
    t.exec();
}
/*!
 * \brief EncryptDialog::zip Zipping algorithm
 * It copresses the data and then compresses it using qCompress()
 * \return Returns Compressed data.
 * \sa ModelPC::unzip
 */
QByteArray EncryptDialog::zip()
{
    // Zip
    QByteArray new_data;
    QByteArray p_data = data;
    if(key)
        p_data = qCompress(data, 9);
    else
        key = data.size();
    // Encrypt
    QByteArray bkey = QByteArray::fromHex(QByteArray::number(key, 16));
    for(long long int i = 0; i < p_data.size(); i++)
        new_data.append(p_data.at(i) ^ bkey.at(i % bkey.size()));
    return new_data;
}
/*!
 * \brief EncryptDialog::on_fileButton_clicked Slot to select the image.
 */
void EncryptDialog::on_fileButton_clicked()
{
    // Selet file
    inputFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    ui->fileLabel->setText(inputFileName);
    // Open image
    QPixmap pixmap(inputFileName);
    // Get size
    size = pixmap.width() * pixmap.height();
    // UI setup
    long long int compr_data_size = compr_data.size();
    ui->zippedBytes->setText(QString::number(compr_data_size));
    if(inputFileName.isEmpty()) {
        ui->percentage->setText("-");
        return;
    }
    double perc = (compr_data_size + 16) * 100 / (size * 3) * bitsUsed / 8;
    ui->percentage->setText(QString::number(perc) + "%");
    goodPercentage = perc < 70;
}
/*!
 * \brief EncryptDialog::on_buttonBox_accepted Slot to start the encryption.
 * Successful closing of the app.
 */
void EncryptDialog::on_buttonBox_accepted()
{
    if(!goodPercentage) {
        alert("Your encoding percentage is over 70% which is a bit ambiguous :(");
        success = false;
        return;
    }
    // Final zip
    compr_data = zip();
    success = true;
    close();
}
/*!
 * \brief EncryptDialog::on_buttonBox_rejected Slot to reject the encryption.
 */
void EncryptDialog::on_buttonBox_rejected()
{
    success = false;
    close();
}
/*!
 * \brief EncryptDialog::on_horizontalSlider_valueChanged Slot if value of the slider is changded.
 * Key is generated here.
 * \param value Value of the slider.
 */
void EncryptDialog::on_horizontalSlider_valueChanged(int value)
{
    // Key generate
    key_size = pow(2, value);
    key = qrand() % key_size + 1;
    val = value;
    ui->keyLabel->setText(QString::number(value));
}

void EncryptDialog::on_bitsSlider_valueChanged(int value)
{
    bitsUsed = value;
    ui->bitsUsedLbl->setText(QString::number(value));
    if(ui->percentage->text() == "-")
        return;
    double perc = (compr_data.size() + 16) * 100 / (size * 3) * 8 / bitsUsed;
    ui->percentage->setText(QString::number(perc) + "%");
}
