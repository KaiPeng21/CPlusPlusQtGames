#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "constants.h"
#include <QQueue>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void initGame();
    void enqueue_snake_head(int x, int y);
    void dequeue_snake_tail();
    void display_snake_coordinate();
    bool is_egg(int x, int y);
    bool egg_eaten();
    void generate_egg();
    bool game_over_check(int x, int y);
    bool is_snake_body(int x, int y);
    bool is_snake(int x, int y);
    bool is_block(int x, int y);

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent * event);

public slots:
    void gameLoop();

private:
    Ui::Dialog *ui;

    int direction;
    int play_status;
    int score;
    Coordinate egg;
    QQueue<Coordinate *> sque;

};

#endif // DIALOG_H
