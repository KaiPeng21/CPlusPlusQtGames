#include "overscreen.h"
#include "game.h"

extern Game * g;

Overscreen::Overscreen(QGraphicsItem *parent): QGraphicsRectItem(parent)
{
    setRect(0,0,SIZE*WIDTH,SIZE*HEIGHT/2);
    setPos(0,0);
    QColor overback(115,115,115,180);
    setBrush(QBrush(overback, Qt::SolidPattern));

    overtext = new QGraphicsTextItem();
    overtext->setDefaultTextColor(Qt::yellow);
    overtext->setFont(QFont("times",48,200,false));
    overtext->setPlainText("GAME OVER");
    overtext->setPos(WIDTH*SIZE/4,HEIGHT*SIZE/3);
    overtext->setZValue(INT_MAX);

    hide_screen();
}

void Overscreen::hide_screen()
{
    setPos(-SIZE*WIDTH-SIZE,SIZE*HEIGHT/4);
    overtext->setPos(-SIZE*WIDTH-SIZE + SIZE*WIDTH/5,SIZE*HEIGHT/4 + SIZE*HEIGHT/5);
}

void Overscreen::show_screen()
{
    setPos(0,SIZE*HEIGHT/4);
    overtext->setPos(SIZE*WIDTH/5, SIZE*HEIGHT/4 + SIZE*HEIGHT/5);
}
