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
    ui->versionLabel->setText("Version " + version);
}
