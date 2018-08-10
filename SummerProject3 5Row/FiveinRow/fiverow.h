#ifndef FIVEROW_H
#define FIVEROW_H

#include <QDialog>

#include "constant_fiverow.h"
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QList>
#include <QStack>
#include <QString>
#include <QColor>
#include <QFont>
#include <QTime>
#include <QThread>
#include <QPushButton>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QTextEdit>

class FiveRow : public QDialog
{
    Q_OBJECT

public:
    FiveRow(QWidget *parent = 0);
    ~FiveRow();

    void initGame();
    void set_matrix();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    bool is_border(int x, int y);
    bool is_black(int x, int y);
    bool is_white(int x, int y);
    bool is_empty(int x, int y);
    bool is_cursor(int x, int y);

    void place();
    void backMove();

    void check();
    int check_horizontal();
    int check_vertical();
    int check_diagonal();

    bool single_check(int x, int y);
    int single_horizontal_check(int x, int y);
    int single_vertical_check(int x, int y);

    void push_moves(int x, int y);
    Coordinate *pop_moves();

    void PQ_DFS_Enqueue(int x, int y, int p);
    AI_Coord *PQ_DFS_Dequeue();

    void PQ_OFS_Enqueue(int x, int y, int p);
    AI_Coord *PQ_OFS_Dequeue();

    bool AI_Random();
    bool AI_Player_Place();
    void AI_Computer_Place();
    void AI_backMove();

    void AI_defense();
    void AI_vertical_defense(int x, int y);
    void AI_horizontal_defense(int x, int y);
    void AI_diagonal_defense(int x, int y);

    void AI_offense();
    void AI_vertical_offense(int x, int y);
    void AI_horizontal_offense(int x, int y);
    void AI_diagonal_offense(int x, int y);

    void AI_ultimate_defense();
    void AI_ultimate_offense();

    void get_help();

protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent * e);

signals:
    void clocking();

public slots:
    void undo_slot();

private:
    int status;
    bool turn;
    Coordinate cursor;
    int Type[LENGTH][LENGTH];
    QList <Coordinate *> moves;
    int result;

    QList <AI_Coord *> PQ_DFS;
    QList <AI_Coord *> PQ_OFS;

};

#endif // FIVEROW_H
