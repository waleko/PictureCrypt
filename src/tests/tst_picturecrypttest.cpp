#include <QtTest>
#include <QCoreApplication>
#include <QByteArray>
#include <QImage>
#include <QObject>
#include <QString>

#include "../app/modelpc.h"
#include "../app/modelpc.cpp"

// add necessary includes here

class PictureCryptTest : public QObject
{
    Q_OBJECT

private:
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
    void test_data()
    {
        QTest::addColumn<QByteArray>("data");
        QTest::addColumn<QImage>("image");
        QTest::addColumn<QString>("expected");
        QTest::addColumn<int>("mode");
        QTest::addColumn<QString>("key");
        QTest::addColumn<int>("bitsUsed");

        QTest::newRow("ok_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 1 << "great_key" << 8;
        QTest::newRow("ok_normal_5") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 1 << "turtles" << 5;
        QTest::newRow("ok_normal_1") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 1 << "fewubhnjimak" << 1;
        QTest::newRow("ok_advanced_1") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 2 << "roger dodger" << 8;
        QTest::newRow("bigdata_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/tinypicture.png")
                                     << "bigdata" << 1 << "qwerty" << 1;
        QTest::newRow("bigdata_advanced_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/tinypicture.png")
                                     << "bigdata" << 2 << "banana" << 1;
        QTest::newRow("nodata_normal_8") << QByteArray("")
                                     << QImage("://test_files/bigpicture.png")
                                     << "nodata" << 1 << "pths" << 1;
        QTest::newRow("nullimage_advanced_8") << read("://test_files/longtext.jpg")
                                     << QImage("invalid")
                                     << "nullimage" << 2 << "penguin" << 1;
        QTest::newRow("bitsWrong_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "bitsWrong" << 1 << "purple" << 12;
        QTest::newRow("no_key_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "no_key" << 1 << "" << 8;
        QTest::newRow("wrongmode_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "wrongmode" << 5 << "mom, get the camera!" << 8;
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
        QImage * retImage = ModelPC::Encrypt(data, &image, mode, key, bitsUsed, &error1);
        // De-embedding
        QByteArray output = ModelPC::Decrypt(retImage, key, mode, &error2);

        // Success of error outputs
        bool er1 = error1 == expected;
        bool er2 = error2 == expected;
        if(expected == "ok") {
            QVERIFY(er1 && er2);
            QCOMPARE(data, output);
        }
        else
            QVERIFY(er1 || er2);
    }
    void test_undefined_decrypt_data()
    {
        QTest::addColumn<QByteArray>("data");
        QTest::addColumn<QImage>("image");
        QTest::addColumn<QString>("expected");
        QTest::addColumn<int>("mode");
        QTest::addColumn<QString>("key");
        QTest::addColumn<int>("bitsUsed");

        QTest::newRow("ok_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 1 << "great_key" << 8;
        QTest::newRow("ok_normal_5") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 1 << "turtles" << 5;
        QTest::newRow("ok_normal_1") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 1 << "fewubhnjimak" << 1;
        QTest::newRow("ok_advanced_1") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "ok" << 2 << "roger dodger" << 8;

    }
    void test_undefined_decrypt()
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
        QImage * retImage = ModelPC::Encrypt(data, &image, mode, key, bitsUsed, &error1);
        // De-embedding
        QByteArray output = ModelPC::Decrypt(retImage, key, 0, &error2);

        // Success of error outputs
        bool er1 = error1 == expected;
        bool er2 = error2 == expected;
        if(expected == "ok") {
            QVERIFY(er1 && er2);
            QCOMPARE(data, output);
        }
        else
            QVERIFY(er1 || er2);
    }
    void test_all_modes_fail()
    {
        QString error;
        QByteArray output = ModelPC::Decrypt(new QImage("://test_files/bigpicture.jpg"), "wow, such code", 0, &error);
        QCOMPARE(error, "all_modes_fail");
    }
    void test_veriffail_data()
    {
        QTest::addColumn<QByteArray>("data");
        QTest::addColumn<QImage>("image");
        QTest::addColumn<QString>("expected");
        QTest::addColumn<int>("mode");
        QTest::addColumn<QString>("key");
        QTest::addColumn<int>("bitsUsed");

        QTest::newRow("veriffail_normal_8") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "veriffail" << 1 << "great_key" << 8;
        QTest::newRow("veriffail_normal_5") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "veriffail" << 1 << "turtles" << 5;
        QTest::newRow("veriffail_normal_1") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "veriffail" << 1 << "fewubhnjimak" << 1;
        QTest::newRow("veriffail_advanced_1") << read("://test_files/longtext.txt")
                                     << QImage("://test_files/bigpicture.jpg")
                                     << "veriffail" << 2 << "roger dodger" << 1;
    }
    void test_veriffail()
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
        QImage * retImage = ModelPC::Encrypt(data, &image, mode, key, bitsUsed, &error1);
        // De-embedding
        QByteArray output = ModelPC::Decrypt(retImage, key + ", btw, I love cookies with tea. :D", mode, &error2);

        QCOMPARE(error1, "ok");
        QCOMPARE(error2, "veriffail");
    }
};

QTEST_MAIN(PictureCryptTest)
#include "tst_picturecrypttest.moc"
