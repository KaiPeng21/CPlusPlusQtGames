#ifndef PLAYER_H
#define PLAYER_H

#include "constant.h"



class Player: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT

public:
    Player(QGraphicsItem * parent = 0);

    bool touchLeftWall();
    bool touchRightWall();
    bool touchGround();

    void start_jump();
    void end_jump();
    void down_jump();

    void move_left();
    void move_right();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

public slots:
    void jump_update();
    void update_pixmap();
    void update_player_in_honey_state();
    void keep_player_in_focus();

public:
    double velocityY;
    double gravity;
    bool onGround;
    bool leftMove;
    bool rightMove;

    bool phs;
    double tmp_styv;
};

#endif // PLAYER_H
