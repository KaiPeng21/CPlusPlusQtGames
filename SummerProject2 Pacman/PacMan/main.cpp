#include "pacmangame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Pacmangame w;
    w.show();

    return a.exec();
}
