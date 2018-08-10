#include "testpc.h"

TestPC::TestPC()
{
    model = new ModelPC();
}

bool TestPC::test(QByteArray data, QImage rImage, QString expectedOutput, int mode, QString key, int bitsUsed)
{
    QString error1, error2;
    QImage * retImage = model->start(data, &rImage, mode, key, bitsUsed, &error1);
    QByteArray output = model->decrypt(retImage, &error2);

    bool er1 = error1 == expectedOutput;
    bool er2 = error2 == expectedOutput;
    if(expectedOutput == "ok")
        return er1 && er2 && data == output;
    else
        return er1 || er2;
}

int TestPC::startTest()
{
    // Long text open
    QFile file(":/unit_tests/longtext.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        return -1;
    }
    text = file.readAll();
    file.close();
    // Big picture open
    image = QImage(":/unit_tests/bigpicture.jpg");

    // Testing
    int sum = 0;
    // The tests
    // Standard tests
    sum += test(text, image);
    sum += test(text, image, "ok", 0, "", 7);
    sum += test(text, image, "ok", 0, "", 1);

    // Error tests
    sum += test(text, QImage(":/unit_tests/tinypicture.png"), "muchdata", 0, "", 8);
    sum += test(QByteArray(), image, "nodata", 0, "", 8);
    sum += test(text, image, "bigkey", 0, QString(text), 8);
    return sum;
}

