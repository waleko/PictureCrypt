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
#include "aboutpc.h"
#include "ui_aboutpc.h"

AboutPC::AboutPC(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPC)
{
    ui->setupUi(this);
}

AboutPC::~AboutPC()
{
    delete ui;
}
/*!
 * \brief AboutPC::setVersion Function to set the version display
 * \param version Version as QString
 */
void AboutPC::setVersion(QString version)
{
    ui->versionLabel->setText(tr("Version ") + version);
}
