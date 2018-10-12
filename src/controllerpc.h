#ifndef CONTROLLERPC_H
#define CONTROLLERPC_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QMessageBox>

#include <modelpc.h>
#include <viewpc.h>
#include <unit_tests/testpc.h>
/*! \file controllerpc.h
 * Header of ControllerPC class
 * \sa ControllerPC, ModelPC, ViewPC
*/
/*!
 * \brief The ControllerPC class
 * Controller class, which controls View and Model layers.
 * \sa ViewPC, ModelPC
 */
class ControllerPC : public QObject
{
    Q_OBJECT
public:
    ControllerPC();
    /*!
     * \brief version Version of the app
     */
    long int version;
    /*!
     * \brief versionString Version of the app as QString.
     */
    QString versionString;
public slots:
    void abortCircuit();
    void runTests();
    void setJPHSDir(QString dir);
private:
    ViewPC * view;
    ModelPC * model;
};

#endif // CONTROLLERPC_H
