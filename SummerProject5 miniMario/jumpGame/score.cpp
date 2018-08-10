#include "score.h"
#include "game.h"

extern Game * g;

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent)
{
    score = 0;

    setPlainText("Score: " + QString::number(score));
    setDefaultTextColor(Qt::darkGreen);
    setFont(QFont("times",16, 100, false));
    setPos(SIZE/2,HEIGHT*SIZE-SIZE);
    setZValue(INT_MAX);
}

void Score::increment()
{
    if (g->player->phs){
        setDefaultTextColor(Qt::darkYellow);
        score = score + 3;
    }
    else{
        setDefaultTextColor(Qt::darkGreen);
        score++;
    }

    setPlainText("Score: " + QString::number(score));
}

int Score::getScore()
{
    return score;
}

void Score::reset()
{
    score = 0;
    setPlainText("Score: " + QString::number(score));
}
