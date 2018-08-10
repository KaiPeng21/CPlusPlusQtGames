#ifndef SCORE_H
#define SCORE_H

#include "constant.h"

class Score: public QGraphicsTextItem{
public:
    Score(QGraphicsItem * parent = 0);
    void increment();
    int getScore();
    void reset();


private:
    int score;
};

#endif // SCORE_H
