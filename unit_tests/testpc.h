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
private slots:
    bool test();
    void checkText();
    void getImage();
private:
    ModelPC * model;
    QByteArray text;
    QImage image;

    // Mode 0 tests
    bool stdTest();
    bool sevenTest();
    bool fiveTest();
    bool fourTest();
    bool threeTest();
    bool oneTest();
    bool overSizeTest();
    bool notImagetest();
    // Mode 1 tests
};

#endif // TESTPC_H
