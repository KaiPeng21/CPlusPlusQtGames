#ifndef HONEY_H
#define HONEY_H

#include "constant.h"

class Honey: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT

public:
    Honey(QGraphicsItem * parent = 0);

    void hide_honey();
    void show_honey();
    void set_honey_appear(bool state);
    bool get_honey_appear();
    void set_honey_end_projected_score(int sc);
    int get_honey_end_projected_score();

public slots:
    void update_honey_pos();

private:
    bool honey_appear;
    double gravity;
    double velocityY;

    int honey_end_projected_score;

};

#endif // HONEY_H
