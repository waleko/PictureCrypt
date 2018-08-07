#include "controllerpc.h"

/*! Controller class
 * \brief ControllerPC::ControllerPC Constructor of controller
 * Constructor checks the version of the app and creates Model Class (ModelPC) and View Class (ViewPC).
 * All signals and slots are connected here.
 * \note Version of the app is specified here.
 */
ControllerPC::ControllerPC()
{
    // Version control
    QString _version = "1.2.5";
    versionString = _version;
    auto ver = _version.split(".");
    version = ver[0].toInt() * pow(2, 16) + ver[1].toInt() * pow(2, 8) + ver[2].toInt();
    // Layer creation
    view = new ViewPC();
    model = new ModelPC(version);
    QThread * modelThread = new QThread();
    model->moveToThread(modelThread);
    modelThread->start();

    view->setVersion(versionString);
    view->show();
    // Layer Connection
    connect(view, SIGNAL(encrypt(QByteArray,QImage*,int)), model, SLOT(encrypt(QByteArray,QImage*,int)));
    connect(view, SIGNAL(decrypt(QImage*)), model, SLOT(decrypt(QImage*)));
    connect(view, SIGNAL(abortModel()), this, SLOT(abortCircuit()));
    connect(view, SIGNAL(setBitsUsed(int)), this, SLOT(setBitsUsed(int)));
    connect(view, SIGNAL(setJPHSDir(QString)), this, SLOT(setJPHSDir(QString)));

    connect(model, SIGNAL(alertView(QString,bool)), view, SLOT(alert(QString,bool)));
    connect(model, SIGNAL(saveData(QByteArray)), view, SLOT(saveData(QByteArray)));
    connect(model, SIGNAL(saveImage(QImage*)), view, SLOT(saveImage(QImage*)));
    connect(model, SIGNAL(setProgress(int)), view, SLOT(setProgress(int)));
}
/*!
 * \brief ControllerPC::abortCircuit Slot to be called when ProgressDialog in ViewPC is closed.
 * It flags ModelPC to stop.
 */
void ControllerPC::abortCircuit()
{
    model->success = false;
}
/*!
 * \brief ControllerPC::setBitsUsed Slot to set ModelPC::bitsUsed
 * \param bitsUsed Value
 */
void ControllerPC::setBitsUsed(int bitsUsed)
{
    model->bitsUsed = bitsUsed;
}
/*!
 * \brief ControllerPC::setJPHSDir Sets JPHS default dir
 * \param dir Directory
 */
void ControllerPC::setJPHSDir(QString dir)
{
    model->defaultJPHSDir = dir;
}
