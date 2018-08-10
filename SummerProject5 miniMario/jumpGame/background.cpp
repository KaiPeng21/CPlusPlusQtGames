#include "background.h"

Background::Background(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    Q_UNUSED(parent);

    setPixmap(QPixmap(":/sounds/tree.jpg"));
    setScale(0.7);
    setPos(0,0);
}
