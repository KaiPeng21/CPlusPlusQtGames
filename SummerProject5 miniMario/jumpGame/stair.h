#ifndef STAIR_H
#define STAIR_H

#include "constant.h"

class Stair: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT

public:
    Stair(QGraphicsItem * parent = 0);
    void set_st_velocityY(double val);

public slots:
    void stair_move();

private:
    double st_velocityY;

};



#endif // STAIR_H
