#include "fiverow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FiveRow w;
    w.show();

    return a.exec();
}
