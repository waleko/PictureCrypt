#include "testpc.h"

TestPC::TestPC()
{
    model = new ModelPC();

    connect(model, SIGNAL(alertView(QString,bool)), this, SLOT(alert(QString,bool)));
    connect(model, SIGNAL(saveData(QByteArray)), this, SLOT(checkText(QByteArray)));
    connect(model, SIGNAL(saveImage(QImage*)), this, SLOT(getsImage(QImage*)));
}

bool TestPC::test(QByteArray data, QImage rImage, int mode, QString key, int bitsUsed)
{

}

int TestPC::startTest()
{
    // Long text open
    QFile file(":/unit_tests/longtext.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        return 1;
    }
    text = file.readAll();
    file.close();
    // Big picture open
    image = QImage(":/unit_tests/bigpicture.jpg");

}


