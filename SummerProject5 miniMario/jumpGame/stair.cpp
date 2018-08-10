#include "stair.h"
#include "game.h"

extern Game *g;

Stair::Stair(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    Q_UNUSED(parent);

    //setRect(0,0,4*SIZE,SIZE);
    setPixmap(QPixmap(":/sounds/stair.png"));
    setScale(0.37);

    //st_velocityY = g->stair_Y_velocity;
}

void Stair::set_st_velocityY(double val)
{
    st_velocityY = val;
}

void Stair::stair_move()
{
    if (g->game_is_over()){
        return;
    }

    if (g->player->phs){
        g->stair_Y_velocity = 3;
        set_st_velocityY(3);
    }
    else if (g->score->getScore() >= 100){
        g->stair_Y_velocity = 3;
        set_st_velocityY(3);
    }
    else if (g->score->getScore() >= 50){
        g->stair_Y_velocity = 2.5;
        set_st_velocityY(2.5);
    }
    else if (g->score->getScore() >= 30){
        g->stair_Y_velocity = 1.8;
        set_st_velocityY(1.8);
    }
    else if (g->score->getScore() >= 10){
        g->stair_Y_velocity = 1.5;
        set_st_velocityY(1.5);
    }
    else if (g->score->getScore() >= 3){
        g->stair_Y_velocity = 1.2;
        set_st_velocityY(1.2);
    }



    //qDebug() << "Size:" << QString::number(g->stair_list.size());
    /*for (int i = 0; i < g->stair_list.size(); i++){
        qDebug() << g->stair_list.at(i)->pos().y() << ",";
    }*/


    setPos(x(), y()+st_velocityY);

    if (pos().y() > HEIGHT*SIZE*3/7-st_velocityY/2 && pos().y() < HEIGHT*SIZE*3/7+st_velocityY/2){
        g->stair_random_generate();
        g->score->increment();
    }

    /*
    else if (pos().y() >= (HEIGHT+2)*SIZE){
        delete this;
    }
    */


}

