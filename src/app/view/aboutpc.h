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
#ifndef ABOUTPC_H
#define ABOUTPC_H

#include <QDialog>

namespace Ui {
class AboutPC;
}
/*!
 * \brief The AboutPC class The About Page dialog
 */
class AboutPC : public QDialog
{
    Q_OBJECT

public:
    explicit AboutPC(QWidget *parent = 0);
    ~AboutPC();
    void setVersion(QString version);

private:
    Ui::AboutPC *ui;
};

#endif // ABOUTPC_H
