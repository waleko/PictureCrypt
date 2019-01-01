#include "controllerpc.h"

/*! Controller class
 * \brief ControllerPC::ControllerPC Constructor of controller
 * Constructor runs auto-test for ModelPC, creates Model Class (ModelPC) and View Class (ViewPC).
 * All signals and slots are connected here.
 * \note Version of the app is specified here.
 */
ControllerPC::ControllerPC()
{
    // Layer creation
    view = new ViewPC();
    model = new ModelPC();
    QThread * modelThread = new QThread();
    model->moveToThread(modelThread);
    modelThread->start();

    view->setVersion(model->versionString);
    view->show();

    // Layers Connection
    connect(view, SIGNAL(encrypt(QByteArray,QImage*,int, int)), model, SLOT(inject(QByteArray,QImage*, int, int)));
    connect(view, SIGNAL(decrypt(QImage*,QString,int)), model, SLOT(decrypt(QImage*, QString, int)));
    connect(view, SIGNAL(abortModel()), this, SLOT(abortCircuit()));
    connect(view, SIGNAL(setJPHSDir(QString)), this, SLOT(setJPHSDir(QString)));
    connect(view, SIGNAL(runTests()), this, SLOT(runTests()));

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
 * \brief ControllerPC::runTests Runs tests
 */
void ControllerPC::runTests()
{
    // FIXME remove all of this mess and use QtTest
    bool res = TestPC::Test();
    QMessageBox o;
    o.setText(!res ? "Testing complete! All tests passed." : "Testing failed.");
    o.exec();
}
/*!
 * \brief ControllerPC::setJPHSDir Sets JPHS default dir
 * \param dir Directory
 */
void ControllerPC::setJPHSDir(QString dir)
{
    model->defaultJPHSDir = dir;
}
