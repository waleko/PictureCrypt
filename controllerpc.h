#ifndef CONTROLLERPC_H
#define CONTROLLERPC_H

#include <QObject>
#include <modelpc.h>
#include <viewpc.h>
#include <QString>
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
signals:

public slots:
    void abortCircuit();
private:
    ViewPC * view;
    ModelPC * model;
};

#endif // CONTROLLERPC_H
