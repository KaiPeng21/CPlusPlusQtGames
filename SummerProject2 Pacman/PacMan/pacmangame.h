#ifndef PACMANGAME_H
#define PACMANGAME_H

#include <QDialog>

#include "constant.h"
#include <QList>
#include <QKeyEvent>
#include <QPainter>
#include <QBrush>
#include <QTime>
#include <QTimer>
#include <QString>

class Pacmangame : public QDialog
{
    Q_OBJECT

public:
    Pacmangame(QWidget *parent = 0);
    ~Pacmangame();

    void initGame();
    void initMatrix();

    bool random();
    void Generate_Enemy();
    void addEnemy(int x, int y);
    void Generate_Item();


    bool isPacMan(int x, int y);
    bool isEnemy(int x, int y);
    bool isBlock(int x, int y);
    bool isItem(int x, int y);

    void gameOverCheck();
    void getItemCheck();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    int Manhatton(int x, int y);
    void moveEnemies();
    void removeNeighborEnemy();
    bool isPacNeighbor(int x, int y);

    void checkBoundary();
    void dfs(int x, int y, int ct);
    void breakRight(int x, int y);
    void breakLeft(int x, int y);

    void setTrap();
    bool isTrap(int x, int y);
    void enemyTrapCheck();

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);


private:
    int score;
    int moves;
    bool over;
    Coordinate pac;
    Coordinate item;
    QList <Coordinate *> enemy;
    bool block[WIDTH][HEIGHT];

    int boundary[WIDTH][HEIGHT];

    Coordinate trap;
    int trapCt;
};

#endif // PACMANGAME_H
