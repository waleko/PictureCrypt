#include "testpc.h"
/*!
 * \brief TestPC::TestPC Constructor
 */
TestPC::TestPC()
{ }
/*!
 * \brief TestPC::Test Static function of testing
 * \return Returns result of the testing
 */
int TestPC::Test()
{
    return TestPC().startTest();
}
/*!
 * \brief TestPC::test Function calling TestPC::model for tests
 * \param data Data for test
 * \param rImage Image for test
 * \param expectedOutput Expected output for test ("ok" if everything is well... ok, else errorcode from ErrorsDict.json)
 * \param mode Mode for embedding
 * \param key Key for for test
 * \param bitsUsed Bits Used
 * \return Returns if test is successful
 * \sa TestPC::autoTest, ModelPC::start, ModelPC::decrypt
 */
bool TestPC::test(QByteArray data, QImage rImage, QString expectedOutput, int mode, QString key, int bitsUsed)
{
    // Error outputs
    QString error1, error2;
    // Embedding
    QImage * retImage = model->start(data, &rImage, mode, key, bitsUsed, &error1);
    // De-embedding
    QByteArray output = model->decrypt(retImage, key, &error2);

    // Success of error outputs
    bool er1 = error1 == expectedOutput;
    bool er2 = error2 == expectedOutput;
    if(expectedOutput == "ok")
        return er1 && er2 && data == output;
    else
        return er1 || er2;
}
/*!
 * \brief TestPC::startTest Starts the tests running
 * \note Tests are configured in tests.json
 *
 * \return Returns success of all tests
 * \sa TestPC::autoTests
 */
int TestPC::startTest()
{
    qDebug() << "Testing started...\n";
    model = new ModelPC();

    // Long text open
    QFile file(":/unit_tests/longtext.txt");
    if(!file.open(QFile::ReadOnly))
        return false;
    text = file.readAll();
    file.close();

    // Big picture open
    image = QImage(":/unit_tests/bigpicture.jpg");
    if(image.isNull())
        return false;

    // JSON tests list open
    QFile json_file(":/unit_tests/tests.json");
    QJsonDocument doc;
    if(!json_file.open(QFile::ReadOnly | QFile::Text))
        return false;
    QByteArray readData = json_file.readAll();
    json_file.close();
    doc = QJsonDocument::fromJson(readData);
    // Testing
    return autoTest(doc);
}
/*!
 * \brief TestPC::autoTest Auto testing function
 * Grabs testing parameters from json and runs TestPC::test
 * \note Log files are generated here.
 * \param doc Json document
 * \return Returns success of all tests.
 */
bool TestPC::autoTest(QJsonDocument doc)
{
    // Opening the tests array
    QJsonObject o = doc.object();
    QJsonArray arr = o["tests"].toArray();
    int sum = 0;

    // Info about tests
    QString extraText;
    for(int i = 0; i < arr.size(); i++) {
        // Reading the data
        QJsonObject obj = arr[i].toObject();

        QString t = obj["data"].toString();
        if(t == "/text/")
            t = text;
        QByteArray data = t.toUtf8();

        QString im = obj["image"].toString();
        QImage img(":/unit_tests/" + im);

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
    QFile file("tests.log");
    bool testsSuc = sum == arr.size();
    if(!file.open(QFile::WriteOnly | QFile::Text))
        return testsSuc;
    QDateTime curTime = QDateTime::currentDateTime();
    QString date = curTime.toString("dd.MM.yyyy HH:mm");
    QString logtext = "############################################\n"
                      "####Log file created at " + date + "####\n"
                      "############################################\n\n"
                      "Status: " + (testsSuc ? "All tests completed" : "Tests failed") + "\n\n"
                      "Tests list:\n";
    logtext += extraText;
    file.write(logtext.toUtf8());
    file.close();
    // Cleaning up
    qDebug() << "Testing completed\n";
    delete model;
    return !testsSuc;
}
