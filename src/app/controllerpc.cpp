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
    connect(view, SIGNAL(encrypt(QByteArray, QImage*, int, QString, int)), model, SLOT(encrypt(QByteArray, QImage*, int, QString, int)));
    connect(view, SIGNAL(decrypt(QImage*,QString,int)), model, SLOT(decrypt(QImage*, QString, int)));
    connect(view, SIGNAL(abortModel()), this, SLOT(abortCircuit()));
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
 * \brief ControllerPC::setJPHSDir Sets JPHS default dir
 * \param dir Directory
 */
void ControllerPC::setJPHSDir(QString dir)
{
    model->defaultJPHSDir = dir;
}
