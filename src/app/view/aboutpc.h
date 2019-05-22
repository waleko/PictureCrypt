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
