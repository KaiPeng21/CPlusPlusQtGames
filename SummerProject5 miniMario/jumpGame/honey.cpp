#include "honey.h"
#include "game.h"

extern Game *g;

Honey::Honey(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/sounds/honey.png"));
    setScale(0.1);
    //setRect(0,0,SIZE,SIZE);

    hide_honey();

    gravity = 0.5;
    velocityY = 0;
    honey_appear = false;
}

void Honey::hide_honey()
{
    setPos(-WIDTH*SIZE,-HEIGHT*SIZE);
}

void Honey::show_honey()
{
    if (honey_appear){
        hide_honey();
    }

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int low = 0;
    int high = WIDTH*SIZE-SIZE;
    int x = qrand() % ((high + 1) - low) + low;

    setPos(x,0);
}

void Honey::set_honey_appear(bool state)
{
    honey_appear = state;
}

bool Honey::get_honey_appear()
{
    return honey_appear;
}

void Honey::set_honey_end_projected_score(int sc)
{
    honey_end_projected_score = sc;
}

int Honey::get_honey_end_projected_score()
{
    return honey_end_projected_score;
}

void Honey::update_honey_pos()
{
    if (g->game_is_over()){
        return;
    }

    if (!honey_appear){
        if (g->score->getScore() % 10 == 5  && !g->player->phs){
            show_honey();
            honey_appear = true;
        }
        else{
            hide_honey();
        }
    }
    else{
        if (g->honey_touch_stair_ground() != -1){
            velocityY = 0;
            setPos(x(),g->honey_touch_stair_ground()-SIZE);
        }
        else{
            velocityY += gravity;
            setPos(x(),y()+velocityY);
        }

        if (pos().y() > HEIGHT*SIZE){
            honey_appear = false;
            velocityY = 0;
        }
    }

    if(g->player_get_honey()){
        set_honey_appear(false);
    }

}
