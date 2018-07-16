#include "controllerpc.h"
#include <QApplication>
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
 *
 * \section use How can someone use it?
 * Well... Anyone who wants to securely commuicate. For example your boss watches your inbox, so
 * you do the work and don't chat with your friends about the bar, they've just visited.
 * Using this app you can send them a photo of your desk, saying it's my new working space,
 * but inside the image there is secret message saying "Wanna get another beer tonight? xD".
 * Boss sees this image, but doesn't spot anyhing.
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
 * See source on https://github.com/AlexKovrigin/PictureCrypt
 * If you still have questions, contact us!
 *
 * Visit our site: http://alex.unaux.com
 * Email me at a.kovrigin0@gmail.com
 * \author Alex Kovrigin
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ControllerPC w;

    return a.exec();
}
