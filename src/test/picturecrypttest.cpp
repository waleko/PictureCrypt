#include <QtTest>
#include <QByteArray>
#include <QImage>
#include <QString>

#include <modelpc.h>

class PictureCryptTest : public QObject
{
    Q_OBJECT
private:
    ModelPC * model;
    QByteArray read(QString path)
    {
        QFile file(path);
        if(!file.open(QFile::ReadOnly | QFile::Text)) {
            return nullptr;
        }
        QByteArray res = file.readAll();
        file.close();
        return res;
    }
private slots:
    void initTestCase() {
        model = new ModelPC();
    }
    void test_data()
    {
        QTest::addColumn<QByteArray>("data");
        QTest::addColumn<QImage>("image");
        QTest::addColumn<QString>("expected");
        QTest::addColumn<int>("mode");
        QTest::addColumn<QString>("key");
        QTest::addColumn<int>("bitsUsed");

        QTest::newRow("ok_normal_8") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.jpg")
                                     << "ok" << 1 << "great_key" << 8;
        QTest::newRow("ok_normal_5") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.jpg")
                                     << "ok" << 1 << "turtles" << 5;
        QTest::newRow("ok_normal_1") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.jpg")
                                     << "ok" << 1 << "fewubhnjimak" << 1;
        QTest::newRow("ok_advanced_1") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.jpg")
                                     << "ok" << 2 << "roger dodger" << 8;
        QTest::newRow("muchdata_normal_8") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/tinypicture.png")
                                     << "muchdata" << 1 << "qwerty" << 1;
        QTest::newRow("muchdata_advanced_8") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/tinypicture.png")
                                     << "muchdata" << 2 << "banana" << 1;
        QTest::newRow("nodata_normal_8") << read("")
                                     << QImage(":/test/assets/bigpicture.png")
                                     << "nodata" << 1 << "pths" << 1;
        QTest::newRow("nullimage_advanced_8") << read(":/test/assets/longtext.txt")
                                     << nullptr
                                     << "nullimage" << 2 << "penguin" << 1;
        QTest::newRow("bitsWrong_normal_8") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.png")
                                     << "bitsWrong" << 1 << "purple" << 12;
        QTest::newRow("no_key_normal_8") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.png")
                                     << "no_key" << 1 << "" << 8;
        QTest::newRow("wrongmode_normal_8") << read(":/test/assets/longtext.txt")
                                     << QImage(":/test/assets/bigpicture.png")
                                     << "wrongmode" << 5 << "" << 8;
    }
    void test()
    {
        QFETCH(QByteArray, data);
        QFETCH(QImage, image);
        QFETCH(QString, expected);
        QFETCH(int, mode);
        QFETCH(QString, key);
        QFETCH(int, bitsUsed);

        // Error outputs
        QString error1, error2;
        // Embedding
        QImage * retImage = model->encrypt(data, &image, mode, key, bitsUsed, &error1);
        // De-embedding
        QByteArray output = model->decrypt(retImage, key, mode, &error2);

        // Success of error outputs
        bool er1 = error1 == expected;
        bool er2 = error2 == expected;
        if(expected == "ok")
            QVERIFY((er1 && er2) && data == output);
        else
            QVERIFY(er1 || er2);
    }
};

QTEST_MAIN(PictureCryptTest);
#include "picturecrypttest.moc"
