#include "controllerpc.h"
#include <QApplication>
#include <unit_tests/testpc.h>
/*!
 * \mainpage PictureCrypt
 * Project made using QT Creator on C++
 *
 * \section idea The idea of the project
 * The idea came to me, when I read an article about steganoraphy.
 * I realised, that you can store data in an image in pixels near the border,
 * so noone can see and even if they did, it is practically impossible to decipher the contents.
 * \section real Realisation
 * To create the encrypted image, you need to select any file for encryption, then using EncryptDialog
 * you select the image to store the data. Then output image is generated.
 * \attention Output image format available is .PNG, because .jpg isn't lossless, so the pixels containing data
 * would be seriously simplified and the data damaged. .BMP isn't used, because noone really uses it and .PNG
 * is just compressed .BMP (more or less)
 * \note JPHS support is under development
 *
 * \section use How can someone use it?
 * Well... Anyone who wants to securely commuicate. For example your boss watches your inbox, so
 * you do the work and don't chat with your friends about the bar, they've just visited.
 * Using this app you can send them a photo of your desk, saying it's my new working space,
 * but inside the image there is secret message saying "Wanna get another beer tonight? xD".
 * Boss sees this image, but doesn't spot anyhing.
 * Great example...
 * \section structure Structure of the project.
 * Project is done via MVC Pattern.
 * View and Model layers are totally isolated and run on different threads.
 *
 * Code from controller.cpp
 * \code
 *  view = new ViewPC();
 *  model = new ModelPC(version);
 *  QThread * modelThread = new QThread();
 *  model->moveToThread(modelThread);
 *  modelThread->start();
 * \endcode
 * So when Model is hard-working, View layer is just fine.
 *
 * Layers also have a ton of functions, so here is a scheme, that I was doing for about 10 hours, which demonstrates
 * the most important functions and classes in the project. And everything is clickable here, so try it out!
 * \dotfile mainpage.gv
 * Well... I think you didn't quite understand what is happening here... So hop into my "User-friendly" Documentation!
 *
 * See source on https://github.com/waleko/PictureCrypt
 *
 * \note QAESEncryption class done by [Bricke](https://github.com/bricke)
 *
 * \section ext-use External use
 * ModelPC class can be used externally (without UI)
 * \code
 * #include <modelpc.h>
 * #include <QByteArray>
 * #include <QImage>
 *
 * ...
 *
 * ModelPC * model = new ModelPC(ver);
 * // ver is version of the app, used to check the data structure version
 * // ver is type long and is calculated as if version is "x.y.z" => ver = x * 65536 + y * 256 + z
 * // Default parameter is 2^17 (2.0.0)
 *
 * // Connecting signals
 *
 * // Essential ones
 *
 * model->saveData(QByteArray data)
 * // Used to return the retrieved data
 *
 * model->saveImage(QImage * image)
 * // Used to return the modified image
 *
 * // Extra ones
 *
 * model->alertView(QString message, bool isWarning)
 * // Used for messages to be shown to users
 *
 * model->setProgress(int val)
 * // Used to show user the progress of embedding
 * // -1 indicates the creation of some kind of progress dialog
 * // from 0 to 100 shows the progress
 * // 101 indicates that progress dialog should be closed
 *
 * \endcode
 * \sa ModelPC, ModelPC::ModelPC, ModelPC::saveData, ModelPC::saveImage, ModelPC::alertView, ModelPC::setProgress
 *
 * Avaible methods see here: https://waleko.github.io/PictureCrypt/#external-use or here ModelPC
 *
 * \section jphs-use JPHS use
 * The newer versions of the app have jphs support, but they don't have jphs built in as it is provided under GNU General Public License v3.0,
 * is "for test purposes only" and is illegal in some countries, so...
 * \attention We support JPHS, but we don't use any responsibility for it, we never used or downloaded it, we just used .exe output in the web,
 * and it somehow works by chance. All responsibility for using jphs is on you, that is why we use made only optionally.
 * That means that to use jphs with our app you will have to download the jphs yourself and specify the jphs directory.
 * However we provide link to the site where you can download the supported version of the jphs: http://linux01.gwdg.de/~alatham/stego.html
 * As it's not our site publishing the dangerous zip file, we just put link to that site (Google does that too, so what? Sue Google?),
 * This text is subject to United Nations' Universal Declaration of Human Rights, (see Article 19 http://www.un.org/en/universal-declaration-human-rights):
 * > Everyone has the right to freedom of opinion and expression; this right includes freedom to hold opinions without interference and to seek, receive and impart information and ideas through any media and regardless of frontiers.
 * And I typed this link randomly, and I'm scared...
 *
 * \section lic License
 * This software is provided under the [UNLICENSE](http://unlicense.org)
 *
 * \section contact Contact us
 * Visit our site: http://alex.unaux.com and Github Page: https://waleko.github.io
 * Email me at a.kovrigin0@gmail.com
 *
 * \author Alex Kovrigin ![me](https://avatars2.githubusercontent.com/u/24986722?s=100)
 * \copyright Alex Kovrigin 2018
 */
// TODO update main page
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestPC test;
    bool success = test.startTest();
    if(success)
        ControllerPC w;

    return a.exec();
}
