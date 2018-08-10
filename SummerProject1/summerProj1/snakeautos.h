#ifndef SNAKEAUTOS_H
#define SNAKEAUTOS_H

#include <QDialog>

#include "constant.h"
#include <QDebug>
#include <QQueue>
#include <QPaintEvent>
#include <QPainter>
#include <QString>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QFont>

class SnakeAutoS : public QDialog
{
    Q_OBJECT

public:
    explicit SnakeAutoS(QWidget *parent = 0);
    ~SnakeAutoS();

    void initGame();
    int get_next_x();
    int get_next_y();

    void auto_initGame();

    void Snake_Enqueue(int x, int y);
    void Snake_Dequeue();
    void Generate_Egg();
    bool is_block(int x, int y);
    bool is_snake_body(int x, int y);
    bool is_snake(int x, int y);
    bool is_egg(int x, int y);
    bool egg_eaten();
    bool game_over();

    bool is_snake_head(int x, int y);
    void Path_Enqueue(int x, int y);
    void Path_Dequeue();
    void setPrev(int x, int y, int px, int py);
    void init_Matrix();
    void BFS();

public slots:
    void game_loop();
    void auto_loop();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QQueue <Coordinate *> sque;
    Coordinate egg;
    int score;
    int direction;
    int play_status;

    int Weight[WIDTH][HEIGHT];
    bool Visited[WIDTH][HEIGHT];
    Coordinate Prev[WIDTH][HEIGHT];
    QQueue <Coordinate *> Path;
};

#endif // SNAKEAUTOS_H
