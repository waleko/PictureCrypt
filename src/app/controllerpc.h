#ifndef CONTROLLERPC_H
#define CONTROLLERPC_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QMessageBox>

#include "model/modelpc.h"
#include "view/viewpc.h"
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
    void setJPHSDir(QString dir);
private:
    ViewPC * view;
    ModelPC * model;
};

#endif // CONTROLLERPC_H
