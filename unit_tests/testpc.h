#ifndef TESTPC_H
#define TESTPC_H

#include <QObject>
#include <modelpc.h>

#include <QFile>
#include <QString>
#include <QImage>
#include <QByteArray>

class TestPC : public QObject
{
    Q_OBJECT
public:
    TestPC();

signals:

public slots:
    int startTest();
protected slots:
    bool test(QByteArray data, QImage rImage,
              QString expectedOutput = "ok", int mode = 0,
              QString key = "", int bitsUsed = 8);
private:
    ModelPC * model;
    QByteArray text;
    QImage image;

    QByteArray returned_text;

};

#endif // TESTPC_H
