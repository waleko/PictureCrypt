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
    key = "";
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
    QByteArray c_data = qCompress(data, 9);
    // Encryption
    QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha256);
    return QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::ECB, c_data, hashKey);
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
    QImage img(inputFileName);
    image = img;
    // Get size
    size = img.width() * img.height();
    // UI setup
    long long int compr_data_size = compr_data.size();
    ui->zippedBytes->setText(QString::number(compr_data_size));
    if(inputFileName.isEmpty()) {
        ui->percentage->setText("");
        return;
    }
    double perc = (compr_data_size + 14) * 100 / (size * 3) * bitsUsed / 8;
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
    key = ui->keyLine->text();
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
 * \brief EncryptDialog::on_bitsSlider_valueChanged Slot if value of the bits slider is changed
 * \param value Well, value
 */
void EncryptDialog::on_bitsSlider_valueChanged(int value)
{
    bitsUsed = value;
    ui->bitsUsedLbl->setText(QString::number(value));
    if(ui->percentage->text().isEmpty())
        return;
    double perc = (compr_data.size() + 14) * 100 / (size * 3) * 8 / bitsUsed;
    ui->percentage->setText(QString::number(perc) + "%");
}
