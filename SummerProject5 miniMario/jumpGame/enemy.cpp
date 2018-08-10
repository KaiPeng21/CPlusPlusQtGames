#include "enemy.h"
#include "game.h"

extern Game *g;


Enemy::Enemy(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/sounds/enemy_right.png"));
    setScale(0.15);

    show_enemy();

    gravity = 0.5;
    velocityY = 0;
    dirX = RIGHT;
}

void Enemy::hide_enemy()
{
    setPos(-WIDTH*SIZE,-HEIGHT*SIZE);
}

void Enemy::show_enemy()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int low = 0;
    int high = WIDTH*SIZE-SIZE;
    int x = qrand() % ((high + 1) - low) + low;

    if (x < WIDTH*SIZE/2){
        dirX = RIGHT;
    }
    else{
        dirX = LEFT;
    }

    setPos(x,0);
}

void Enemy::update_enemy_pixmap()
{
    if (dirX == RIGHT){
        setPixmap(QPixmap(":/sounds/enemy_right.png"));
    }
    else{
        setPixmap(QPixmap(":/sounds/enemy_left.png"));
    }
}

void Enemy::update_enemy_pos()
{
    if (g->game_is_over()){
        return;
    }


    if (dirX == RIGHT && pos().x() < SIZE*(WIDTH-1)){
        setPos(x() + 1, y());
    }
    if (dirX == LEFT && pos().x() > 0){
        setPos(x() - 1,y());
    }
    if (pos().x() >= SIZE*(WIDTH-1)){
        dirX = LEFT;
    }
    if (pos().x() <= 0){
        dirX = RIGHT;
    }



    if (g->enemy_touch_stair_ground() != -1){
        velocityY = 0;
        setPos(x(), g->enemy_touch_stair_ground() - SIZE);
    }
    else{
        velocityY += gravity;
        setPos(x(), y() + velocityY);
    }
    if (pos().y() >= SIZE*HEIGHT){
        show_enemy();
    }



}
