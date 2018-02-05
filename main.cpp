#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller c;
    c.loadWindows();
    return a.exec();
}
