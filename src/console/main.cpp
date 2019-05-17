#include <QCoreApplication>
#include <QImage>
#include <QByteArray>
#include <QString>
#include <bits/stdc++.h>
#include <QFile>
#include "../app/model/modelpc.h"

using namespace std;
/*!
 * \brief printHelp Shows help
 */
void printHelp() {
    cout << "Usage:\n"
            "  picturecrypt encrypt <image> <input file> <key> <output> [options]\n"
            "  picturecrypt decrypt <image> <key> <output> [options]\n"
            "  picturecrypt (-h | --help)\n"
            "  picturecrypt --version\n\n"
            "Options:\n"
            "  -h --help    Show this screen.\n"
            "  --version    Show version.\n"
            "  -m --mode    Mode of cryption.\n"
            "  -b           Encryption bitsUsed parameter.\n";
}
/*!
 * \brief main Main PictureCrypt function
 * \param argc Parameters count
 * \param argv Parameters array
 * \return Returns exit code
 */
int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    if (argc <= 1) {
        printHelp();
    }
    else if (QString(argv[1]) ==  "encrypt") {
        if(argc < 6) {
            cout << "Not enough parameters, see help with 'picturecrypt -h'" << endl;
            return 1;
        }
        QImage image((QString(argv[2])));
        if(image.isNull()) {
            cout << "Image path is invalid!" << endl;
            return 2;
        }
        QFile file((QString(argv[3])));
        if(!file.open(QFile::ReadOnly)) {
            cout << "Cannot open input data!" << endl;
            return 3;
        }
        QString key(argv[4]);
        QString outPath(argv[5]);

        int mode = 2;
        int bitsUsed = 8;
        for(int i = 6; i < argc; i++)
        {
            if((QString(argv[i]) == "-m" || QString(argv[i]) == "--mode") && i + 1 != argc)
                mode = QString(argv[++i]).toInt();
            else if((QString(argv[i]) == "-b" || QString(argv[i]) == "--bitsUsed") && i + 1 != argc)
                bitsUsed = QString(argv[++i]).toInt();
        }
        QString error;
        QImage *ret = ModelPC::Encrypt(file.readAll(), &image, mode, key, bitsUsed, &error);
        if(error != "ok") {
            cout << "Critical error: " << error.toStdString() << endl;
            return 4;
        }
        if(!ret->save(outPath)) {
            cout << "Output path is incorrect!" << endl;
            return 5;
        }
    }
    else if (QString(argv[1]) == "decrypt") {
        if(argc < 5) {
            cout << "Not enough parameters, see help with 'picturecrypt -h'" << endl;
            return 1;
        }
        QImage image((QString(argv[2])));
        if(image.isNull()) {
            cout << "Image path is invalid!" << endl;
            return 2;
        }
        QString key(argv[3]);
        QString outPath(argv[4]);
        int mode = 0;
        for(int i = 5; i < argc; i++)
        {
            if((QString(argv[i]) == "-m" || QString(argv[i]) == "--mode") && i + 1 != argc)
                mode = QString(argv[++i]).toInt();
        }
        QString error;
        QByteArray data = ModelPC::Decrypt(&image, key, mode, &error);
        if(error != "ok") {
            cout << "Critical error: " << error.toStdString() << endl;
            return 4;
        }

        QFile writeFile(outPath);
        if (!writeFile.open(QIODevice::WriteOnly))
        {
            cout << "Output path is incorrect!" << endl;
            return 5;
        }
        writeFile.write(data);
        writeFile.close();
    }
    else if(QString(argv[1]) == "--version") {
        ModelPC ver;
        cout << ver.versionString.toStdString();
    }
    else {
        // cout << "Help: " << argv[1] << endl;
        if(QString(argv[1]) != "-h" && QString(argv[1]) != "--help")
            cout << "Incorrect use. See help." << endl;
        printHelp();
    }

    return 0;
}
