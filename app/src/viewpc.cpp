#include "viewpc.h"
#include "ui_viewpc.h"

ViewPC::ViewPC(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewPC)
{
    ui->setupUi(this);

    progressDialogClosed = true;

    setupErrorsDict();

    isEncrypt = true;
}
/*!
 * \brief ViewPC::~ViewPC Simple destructor for this layer
 */
ViewPC::~ViewPC()
{
    delete ui;
}

void ViewPC::on_encryptMode_clicked()
{
    // Encrypt radio button clicked
    setEncryptMode(true);
}

void ViewPC::on_decryptMode_clicked()
{
    // Decrypt radio button clicked
    setEncryptMode(false);
}
/*!
 * \brief ViewPC::on_fileButton_clicked Slot to be called, when according button is pressed.
 */
void ViewPC::on_fileButton_clicked()
{
    // Opening QFileDialog depending on isEncrypt
    if(isEncrypt)
        inputFileName = QFileDialog::getOpenFileName(this, tr("Select file"), "/untitled.txt", tr("Text files (*.txt);;All Files (*)"));
    else
        inputFileName = QFileDialog::getOpenFileName(this, tr("Select file"), "/untitled.png", tr("PNG files (*.png);;All Files (*)"));
    // Display the file name
    ui->fileLabel->setText(inputFileName.isEmpty() ? tr("File not chosen") : inputFileName);
}
/*!
 * \brief ViewPC::on_startButton_clicked Slot to be called, when Start Button is pressed.
 * \section encrypting Encrypting
 * If Encrypting mode is active the data from text browser or from file from file selector
 * will be opened and checked in size.
 * \note File size limit is 16MB
 *
 * Then the EncryptDialog opens and image and key is selected.
 * Then the ViewPC::encrypt signal is called to start ModelPC::encrypt
 * \section decrypting Decrypting
 * Else, the image from file selector is transmitted to ModelPC::decrypt
 */
void ViewPC::on_startButton_clicked()
{
    if(isEncrypt)
    {
        // Getting the data
        QString text = ui->text->toPlainText();
        QByteArray data;
        if(text.isEmpty()) {
            if(inputFileName.isEmpty()) {
                alert("no_input_file", true);
                return;
            }
            // Opening the file
            QFile file(inputFileName);
            if (!file.open(QIODevice::ReadOnly))
            {
                alert("open_file_fail", true);
                return;
            }
            // Check the data size
            auto size = file.size();
            if(size > qPow(2, 24)) {
                alert("muchdata", true);
                file.close();
                return;
            }
            data = file.readAll();
            file.close();
        }
        else
            data = text.toUtf8();
        // Select image via EncryptDialog
        EncryptDialog * dialog = new EncryptDialog(data);
        dialog->exec();
        if(!dialog->success)
            return;

        // Get the data
        QByteArray encr_data = dialog->compr_data;

        // Save the hash
        QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
        encr_data = hash + encr_data;

        emit encrypt(data, &dialog->image, selectedMode, dialog->key, dialog->bitsUsed);
    }
    else
    {
        // Get the filename of the image
        if(inputFileName.isEmpty()) {
            alert("no_input_file", true);
            return;
        }
        QByteArray key = requestKey().toUtf8();
        if(key.isEmpty())
            return;
        QImage * res_image = new QImage(inputFileName);
        emit decrypt(res_image, key, 0);
    }
}
/*!
 * \brief ViewPC::alert Slot to create QMessageBox with message
 * \param message Message to be shown
 * \param isWarning Flag, if message is critical.
 */
void ViewPC::alert(QString message, bool isWarning)
{
    // Get message
    if(errorsDict.contains(message))
        message = errorsDict[message];
    // Create message box
    QMessageBox box;
    if(isWarning)
        box.setIcon(QMessageBox::Warning);
    else
        box.setIcon(QMessageBox::Information);
    box.setText(message);
    box.setWindowIcon(QIcon(":/icons/mail.png"));
    box.setWindowTitle(tr("Message"));
    box.exec();
}
/*!
 * \brief ViewPC::saveData Slot to be called to save data using QFileDialog.
 * \param Edata Encrypted data to be saved.
 * \sa ModelPC::encrypt
 */
void ViewPC::saveData(QByteArray Edata)
{
    // Save data using QFileDialog
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "/untitled.txt",
                               tr("Text(*.txt);;All files (*)"));
    QFile writeFile(outputFileName);
    if (!writeFile.open(QIODevice::WriteOnly))
    {
        alert("save_file_fail", true);
        return;
    }
    writeFile.write(Edata);
    writeFile.close();
    alert("decryption_completed");
}
/*!
 * \brief ViewPC::saveImage Slot to be called to save image using QFileDialog.
 * \param image Image to be saved.
 * \sa ModelPC::decrypt
 */
void ViewPC::saveImage(QImage * image)
{
    // Save image using QFileDialog
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save Image"),
                               "/untitled.png",
                               tr("Images(*.png)"));
    if(!image->save(outputFileName)) {
        alert("save_file_fail", true);
        return;
    }
    alert("encryption_completed");
}
/*!
 * \brief ViewPC::setProgress Slot to set the value of the ProgressDialog (ViewPC::dialog).
 * \param val New value of the dialog.
 * If -1, creates ProgressDialog, if 101 closes the dialog.
 * \sa ViewPC::abortCircuit(), ModelPC::setProgress()
 */
void ViewPC::setProgress(int val)
{
    if(val < 0) {
        // Create dialog
        dialog = new QProgressDialog(tr("Cryption in progress."), tr("Cancel"), 0, 100);
        connect(dialog, SIGNAL(canceled()), this, SLOT(abortCircuit()));
        progressDialogClosed = false;
        dialog->setWindowTitle(tr("Processing"));
        dialog->setWindowIcon(QIcon(":/icons/loading.png"));
        dialog->show();
    }
    else if(val > 100 && !progressDialogClosed) {
        // Close dialog
        dialog->setValue(100);
        QThread::msleep(25);
        dialog->close();
        dialog->reset();
        progressDialogClosed = true;
    }
    // Update the progress
    else if(!progressDialogClosed)
        dialog->setValue(val);
}
/*!
 * \brief ViewPC::abortCircuit Slot to close ProgressDialog (ViewPC::dialog)
 */
void ViewPC::abortCircuit()
{
    // Set the flag
    progressDialogClosed = true;
    // Close the dialog
    dialog->close();
    dialog->reset();
    emit abortModel();
}
/*!
 * \brief ViewPC::setEncryptMode Set the encrpt mode (ViewPC::isEncrypt)
 * \param encr = isEncrypt, true if encrypting, false if decrypting
 */
void ViewPC::setEncryptMode(bool encr)
{
    ui->text->setText("");
    ui->text->setEnabled(encr);
    isEncrypt = encr;
    ui->startButton->setText(encr ? tr("Continue configuration") : tr("Start decryption"));
    ui->enLabel1->setText(encr ? tr("Type in the text for encryption:") : tr("Text input isn't supported in decryption mode"));
    ui->enLabel1->setEnabled(encr);
    ui->enLabel2->setText(encr ? tr("Or use the file dialog to choose a file:") : tr("Choose a file for decryption:"));
    ui->comboBox->setEnabled(encr);
}
/*!
 * \brief ViewPC::setVersion Set the version of the app from ControllerPC
 * \param version Version as QString
 */
void ViewPC::setVersion(QString version)
{
    // Version setup
    versionString = version;
}
/*!
 * \brief ViewPC::requestKey Request keyphrase from user using InputDialog
 * \return Returns keyphrase
 */
QString ViewPC::requestKey()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Dialog"),
                                         tr("Enter the keyphrase:"), QLineEdit::Password,
                                         "", &ok);
    if(text.isEmpty() && ok) {
        alert("no_key", true);
        return QString();
    }
    return ok ? text : QString();
}

QByteArray ViewPC::bytes(long long n)
{
    return QByteArray::fromHex(QByteArray::number(n, 16));
}
/*!
 * \brief ViewPC::on_actionAbout_triggered Opens about page
 */
void ViewPC::on_actionAbout_triggered()
{
    AboutPC about;
    about.setVersion(versionString);
    about.exec();
}

/*!
 * \brief ViewPC::on_actionHelp_triggered Opens online documentation
 */
void ViewPC::on_actionHelp_triggered()
{
    QUrl docLink("https://picturecrypt.alexkovrigin.me");
    QDesktopServices::openUrl(docLink);
}
/*!
 * \brief ViewPC::setupErrorsDict Setups errorsDict from strings.xml
 */
void ViewPC::setupErrorsDict()
{
    errorsDict["no_data"] = tr("No data given!");
    errorsDict["muchdata"] = tr("Data size is too big (must be less than 15MB)!");
    errorsDict["nullimage"] = tr("Invalid / null image!");
    errorsDict["bigimage"] = tr("Image is too big!");
    errorsDict["bitsWrong"] = tr("bitsUsed parameter is wrong!");
    errorsDict["no_key"] = tr("No key given!");
    errorsDict["big_key"] = tr("Given key is too big!");
    errorsDict["undefined_mode"] = tr("Undefined mode is only available when decrypting!");
    errorsDict["wrongmode"] = tr("Given mode isn't available!");
    errorsDict["inject-v1.4"] = tr("ModelPC::inject() isn't available with v1.4 (advanced) mode");
    errorsDict["all_modes_fail"] = tr("Given image isn't encrypted by this app (all modes have failed) or is damaged!");
    errorsDict["nojphs"] = tr("JPHS is not installed!");
    errorsDict["bitsBufferFail"] = tr("bitsBufferFail (holy crap, contact me or submit a bug)");
    errorsDict["point_visited_twice"] = tr("One point visited twice (holy crap, contact me or submit a bug)");
    errorsDict["bigdata"] = tr("Too much data for this image!");
    errorsDict["veriffail"] = tr("Given image isn't encrypted with this mode or is damaged!");
    errorsDict["noreaddata"] = tr("No data to read from image!");
    errorsDict["new_version"] = tr("Version of the image is newer than yours (update!!!)");
    errorsDict["old_version"] = tr("Version of the image is older than yours");
    errorsDict["no_input_file"] = tr("No file given!");
    errorsDict["open_file_fail"] = tr("Cannot open file!");
    errorsDict["save_file_fail"] = tr("Cannot save file!");
    errorsDict["decryption_completed"] = tr("Decryption completed!");
    errorsDict["encryption_completed"] = tr("Encryption completed!");
}

void ViewPC::on_actionJPHS_path_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open JPHS folder"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    emit setJPHSDir(dir);
}

void ViewPC::on_comboBox_currentIndexChanged(int index)
{
    selectedMode = index + 1;
}

void ViewPC::on_text_textChanged()
{
    ui->fileButton->setEnabled(ui->text->toPlainText().isEmpty());
}
