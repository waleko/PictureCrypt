#include "controllerpc.h"

/*! Controller class
 * \brief ControllerPC::ControllerPC Constructor of controller
 * Constructor runs auto-test for ModelPC, creates Model Class (ModelPC) and View Class (ViewPC).
 * All signals and slots are connected here.
 * \note Version of the app is specified here.
 */
ControllerPC::ControllerPC()
{
    // ModelPC auto-test


    // Layer creation
    view = new ViewPC();
    model = new ModelPC();
    QThread * modelThread = new QThread();
    model->moveToThread(modelThread);
    modelThread->start();


    view->setVersion(model->versionString);
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
