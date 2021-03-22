#include <QCoreApplication>
#include <QImage>
#include <QByteArray>
#include <QString>
#include <bits/stdc++.h>
#include <QFile>
#include "../app/model/modelpc.h"

using namespace std;

int parserEncrypt(QCommandLineParser& parser, QCoreApplication& app)
{
    parser.addPositionalArgument("image", QCoreApplication::translate("main", "Source image for embedding."));
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to be hidden."));
    parser.addPositionalArgument("key", QCoreApplication::translate("main", "Key for encryption"));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination image file."));

    // An option with a value
    QCommandLineOption modeOption(QStringList() << "m" << "mode",
            QCoreApplication::translate("main", "Mode for encryption.")); // FIXME
    modeOption.setDefaultValue("2");
    parser.addOption(modeOption);

    QCommandLineOption bitsOption(QStringList() << "b" << "bitsUsed",
            QCoreApplication::translate("main", "Bits used for mode 1 of encryption")); // FIXME
    bitsOption.setDefaultValue("8");
    parser.addOption(bitsOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QImage image(args.at(0));
    if(image.isNull()) {
        qCritical() << "Image path is invalid!\n";
        return 1;
    }
    QFile file(args.at(1));
    if(!file.open(QFile::ReadOnly)) {
        cout << "Cannot open input data!" << endl;
        return 1;
    }
    QString key(args.at(2));
    QString outPath(args.at(3));

    int mode = parser.value(modeOption).toInt();
    int bitsUsed = parser.value(bitsOption).toInt();
    QString error;
    QImage *ret = ModelPC::Encrypt(file.readAll(), &image, ModelPC::CryptMode(mode), key, bitsUsed, &error);
    if(error != "ok") {
        qCritical() << "Cannot proceed! Error code: " << error << endl;
        return 1;
    }
    if(!ret->save(outPath)) {
        qCritical() << "Output path is incorrect" << endl;
        return 1;
    }
    return 0;
}

int parserDecrypt(QCommandLineParser& parser, QCoreApplication& app)
{
    parser.addPositionalArgument("image", QCoreApplication::translate("main", "Source image for embedding."));
    parser.addPositionalArgument("key", QCoreApplication::translate("main", "Key for encryption"));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination decrypted file."));

    // An option with a value
    QCommandLineOption modeOption(QStringList() << "m" << "mode",
            QCoreApplication::translate("main", "Mode for encryption.")); // FIXME
    modeOption.setDefaultValue("0");
    parser.addOption(modeOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QImage image(args.at(0));
    if(image.isNull()) {
        qCritical() << "Image path is invalid!\n";
        return 1;
    }
    QString key(args.at(1));
    QString outPath(args.at(2));

    int mode = parser.value(modeOption).toInt();
    QString error;
    QByteArray data = ModelPC::Decrypt(&image, key, ModelPC::CryptMode(mode), &error);
    if(error != "ok") {
        qCritical() << "Cannot proceed! Error code: " << error << endl;
        return 1;
    }
    QFile writeFile(outPath);
    if (!writeFile.open(QIODevice::WriteOnly))
    {
        cout << "Output path is incorrect" << endl;
        return 1;
    }
    writeFile.write(data);
    writeFile.close();
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("picturecrypt");
    QCoreApplication::setApplicationVersion(ModelPC().versionString);

    QCommandLineParser parser;
    parser.setApplicationDescription("Image steganography utility");
    parser.addHelpOption();
    parser.addVersionOption();

    if (argc < 2) {
        qCritical() << "No command was provided. See help with --help"; // FIXME
        return 1;
    }
    QString command = argv[1];
    if (command == "encrypt")
        return parserEncrypt(parser, app);
    else if (command == "decrypt")
        return parserDecrypt(parser, app);
    else {
        bool isHelp = command == "-?" || command == "--help" || command == "-h";
        cout << isHelp << endl;
        if (isHelp) {
            cout << parser.helpText().toStdString() << endl;
            cout << "Commands:\n"
                    "  encrypt         Hides file contents inside an image.\n"
                    "  decrypt         Retrieves file contents from an image.\n";
            return 0;
        } else {
            qCritical() << "No such command: " << command << ". See help with --help"; // FIXME
            return 1;
        }
    }
}
