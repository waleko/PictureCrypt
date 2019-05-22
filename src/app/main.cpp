#include "controllerpc.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QList<QString> fonts = { "Montserrat-Black.ttf", "Montserrat-BlackItalic.ttf", "Montserrat-Bold.ttf", "Montserrat-BoldItalic.ttf", "Montserrat-Medium.ttf", "Montserrat-MediumItalic.ttf", "Montserrat-Regular.ttf", "Montserrat-Italic.ttf", "Montserrat-Light.ttf", "Montserrat-LightItalic.ttf", "Montserrat-Thin.ttf", "Montserrat-ThinItalic.ttf" };

    foreach(const QString &font, fonts) {
        if(QFontDatabase::addApplicationFont(":/fonts/" + font) == -1)
            qDebug() << "Error loading font: " + font;
    }

    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("picturecrypt"), QLatin1String("_"), QLatin1String(":/translations"))) {
        a.installTranslator(&translator);
    } else {
        qDebug() << "[!!!] cannot load translator " << QLocale::system().name() << " check content of translations.qrc";
    }

    ControllerPC w;
    return a.exec();
}
