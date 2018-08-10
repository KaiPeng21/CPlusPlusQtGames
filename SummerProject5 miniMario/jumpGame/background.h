#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "constant.h"

class Background: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT

public:
    Background(QGraphicsItem * parent = 0);

};


#endif // BACKGROUND_H
