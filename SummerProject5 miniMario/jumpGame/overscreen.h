#ifndef OVERSCREEN_H
#define OVERSCREEN_H

#include "constant.h"

class Overscreen: public QObject, public QGraphicsRectItem{
    Q_OBJECT

public:
    Overscreen(QGraphicsItem * parent = 0);

    void hide_screen();
    void show_screen();

    QGraphicsTextItem * overtext;
};


#endif // OVERSCREEN_H
