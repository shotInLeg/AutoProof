#include "sproductionsystem.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SProductionSystem w;
    w.show();

    return a.exec();
}
