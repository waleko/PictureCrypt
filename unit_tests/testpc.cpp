#include "testpc.h"

TestPC::TestPC()
{
    model = new ModelPC();
}

int TestPC::startTest()
{
    // Long text open
    QFile file(":/unit_tests/longtext.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        return 1;
    }
    text = file.readAll();
    file.close();
    // Big picture open

}
