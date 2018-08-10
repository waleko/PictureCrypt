#include "testpc.h"

TestPC::TestPC()
{ }

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
    model = new ModelPC();

    // Long text open
    QFile file(":/unit_tests/longtext.txt");
    if(!file.open(QFile::ReadOnly))
        return 2;
    text = file.readAll();
    file.close();
    // Big picture open
    image = QImage(":/unit_tests/bigpicture.jpg");
    if(image.isNull())
        return 3;
    // JSON tests list open
    // TODO add relative path
    QFile json_file("C:/Users/salex/Documents/GitHub/PictureCrypt/unit_tests/tests.json");
    if(!json_file.open(QFile::ReadOnly | QFile::Text))
        return 4;
    json_file.close();
    QJsonDocument doc = QJsonDocument::fromJson(json_file.readAll());

    // Testing
    bool success = doc.isEmpty() ? stdTest() : autoTest(doc);
    return success ? 0 : 1;
}

bool TestPC::stdTest()
{
    // TODO make function just json writer and run autotest
    int sum = 0;
    // Tests
    // Standard tests
    sum += test(text, image);
    sum += test(text, image, "ok", 0, "", 7);
    sum += test(text, image, "ok", 0, "", 1);

    // Error tests
    sum += test(text, QImage(":/unit_tests/tinypicture.png"), "muchdata", 0, "", 8);
    sum += test(QByteArray(), image, "nodata", 0, "", 8);
    sum += test(text, image, "bigkey", 0, QString(text), 8);

    return sum == 6;
}

bool TestPC::autoTest(QJsonDocument doc)
{
    QJsonObject o = doc.object();
    QJsonArray arr = o["tests"].toArray();
    int sum = 0;

    QString extraText;
    for(int i = 0; i < arr.size(); i++) {
        // Reading
        QJsonObject obj = arr[i].toObject();

        QString t = obj["data"].toString();
        if(t == "/text/")
            t = text;
        QByteArray data = t.toUtf8();

        QString im = obj["image"].toString();
        // TODO Move all images to test_images
        QImage img(":/unit_tests/test_images" + im);

        QString expect = obj["expectation"].toString();

        int mode = obj["mode"].toInt();

        QString key = obj["key"].toString();

        int bitsUsed = obj["bitsUsed"].toInt();

        // Testing
        bool s = test(data, img, expect, mode, key, bitsUsed);
        sum += s;
        extraText += "\n * Test #" + QString::number(i + 1) + " " + (s ? "completed." : "failed.");
    }
    // Writing log
    QFile file(":/unit_tests/tests.log");
    bool testsSuc = sum == arr.size();
    if(!file.open(QFile::WriteOnly | QFile::Text))
        return testsSuc;
    QDateTime curTime = QDateTime::currentDateTime();
    QString date = curTime.toString("dd.MM.yyyy HH:mm");
    QString logtext = "############################################"
                      "####Log file created at " + date + "####"
                      "############################################"
                      ""
                      "Status: " + (testsSuc ? "All tests completed" : "Test fail") + ""
                      ""
                      "Tests failed:"
                      "";
    logtext += extraText;
    file.write(logtext.toUtf8());
    file.close();
    return testsSuc;
}
