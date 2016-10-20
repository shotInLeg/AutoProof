#include "autoproof.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AutoProof w;
    w.show();

    return a.exec();
}
