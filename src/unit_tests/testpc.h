#ifndef TESTPC_H
#define TESTPC_H

#include <QObject>
#include <modelpc.h>

#include <QFile>
#include <QDebug>
#include <QString>
#include <QImage>
#include <QByteArray>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QDateTime>
/*!
 * \brief The TestPC class AutoTest for ModelPC
 * Currently used in main.cpp
 */
class TestPC : public QObject
{
    Q_OBJECT
public:
    TestPC();
    static int Test();
    // TODO add static Test();
public slots:
    int startTest();
protected slots:
    bool test(QByteArray data, QImage rImage,
              QString expectedOutput = "ok", int mode = 0,
              QString key = "", int bitsUsed = 8);
private:
    /*!
     * \brief model Test model
     */
    ModelPC * model;
    /*!
     * \brief text Standard text for testing (set in longtext.txt)
     */
    QByteArray text;
    /*!
     * \brief image bigpicture.jpg (main image for testing)
     */
    QImage image;

    bool autoTest(QJsonDocument doc);
};

#endif // TESTPC_H
