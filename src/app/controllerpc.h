/***********************************************************************************
 *                                                                                 *
 *   Original work Copyright (C) 2019 by Alex Kovrigin (waleko)                  *
 *                                                                                 *
 *   https://alexkovrigin.me                                                       *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy  *
 *   of this software and associated documentation files (the "Software"), to deal *
 *   in the Software without restriction, including without limitation the rights  *
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
 *   copies of the Software, and to permit persons to whom the Software is         *
 *   furnished to do so, subject to the following conditions:                      *
 *                                                                                 *
 *   The above copyright notice and this permission notice shall be included in    *
 *   all copies or substantial portions of the Software.                           *
 *                                                                                 *
 ***********************************************************************************/
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
