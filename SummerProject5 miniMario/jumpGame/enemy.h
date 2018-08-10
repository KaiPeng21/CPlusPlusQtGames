#ifndef ENEMY_H
#define ENEMY_H

#include "constant.h"

class Enemy: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT

public:
    Enemy(QGraphicsItem * parent = 0);

    void hide_enemy();
    void show_enemy();

public slots:
    void update_enemy_pixmap();
    void update_enemy_pos();


private:
    double gravity;
    double velocityY;
    int dirX;

};


#endif // ENEMY_H
