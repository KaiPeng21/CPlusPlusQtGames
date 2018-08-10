#include <QApplication>

#include "constant.h"
#include "game.h"

Game *g;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g = new Game();
    g->show();

    return a.exec();
}
