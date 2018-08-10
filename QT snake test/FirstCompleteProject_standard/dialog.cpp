#include "dialog.h"
#include "ui_dialog.h"

#include <iostream>
#include <QKeyEvent>
#include <QPainter>
#include <QString>
#include <QTextDocument>
#include <QDebug>
#include <QTimer>
#include <QTime>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{


    // set the screen size
    resize((WIDTH+1)*SIZE, (HEIGHT+1)*SIZE + SIZE*3);
    // initialize the game conditions
    initGame();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QTimer * timer = new QTimer(this);
    // update game data
    connect(timer,SIGNAL(timeout()),this,SLOT(gameLoop()));
    // update graphical display
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(50);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initGame()
{
    direction = UP;
    play_status = PLAYING;
    score = 0;

    // initialize snake
    sque.clear();
    for (int i = 0; i < STARTING_L; i++){
        enqueue_snake_head(WIDTH/2, HEIGHT/2+2 - i);
    }

    // initialize egg
    generate_egg();

    //qDebug() << "front" << sque.front()->y;
    //qDebug() << "back" << sque.back()->y;
    //qDebug() << sque[0]->y << "," << sque[1]->y << "," << sque[2]->y;
    //qDebug() << "size: " << sque.size();

}

void Dialog::enqueue_snake_head(int x, int y)
{
    Coordinate * pos = new Coordinate();
    pos->x = x;
    pos->y = y;
    sque.enqueue(pos);
}

void Dialog::dequeue_snake_tail()
{
    if (!sque.isEmpty()){
        Coordinate * tmp = sque.dequeue();
        delete tmp;
    }
}

void Dialog::display_snake_coordinate()
{
    for (int i = 0; i < sque.size(); ++i) {
        qDebug() << "(" << sque.at(i)->x << " ," << sque.at(i)->y << ")" << " <- ";
    }
    qDebug() << "\n";
}

bool Dialog::is_egg(int x, int y)
{
    if (x == egg.x && y == egg.y){
        return true;
    }
    return false;
}

bool Dialog::egg_eaten()
{
    int shead_x = sque.back()->x;
    int shead_y = sque.back()->y;
    if (is_egg(shead_x, shead_y)){
        return true;
    }
    return false;
}

// randomly set the positon of egg.x and egg.y
// re-set if the position is snake or block
void Dialog::generate_egg()
{
    int egg_x, egg_y;
    do{
        egg_x = qrand() % WIDTH + 1;
        egg_y = qrand() % HEIGHT + 1;
    }while(is_snake(egg_x, egg_y) || is_block(egg_x, egg_y));

    egg.x = egg_x;
    egg.y = egg_y;
}

// return true if game over, false if game is not over
// check if the snake head position is snake body or block
bool Dialog::game_over_check(int x, int y)
{
    if (is_snake_body(x, y) || is_block(x, y)){
        return true;
    }
    return false;
}

// check if the coordinate is a snake (excluding snake head)
bool Dialog::is_snake_body(int x, int y)
{
    if (sque.size() != 0){
        for (int i = 0; i < sque.size()-1; ++i) {
            if (x == sque.at(i)->x && y == sque.at(i)->y){
                return true;
            }
        }
    }
    return false;
}
// check if the coordinate is a snake (including snake head)
bool Dialog::is_snake(int x, int y)
{
    if (is_snake_body(x, y)){
        return true;
    }
    if (x == sque.back()->x && y == sque.back()->y){
        return true;
    }
    return false;
}

bool Dialog::is_block(int x, int y)
{
    if(x == 0 || x == WIDTH || y == 0 || y == HEIGHT){
        return true;
    }
    return false;
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up && direction != DOWN && play_status == PLAYING){
        direction = UP;
        //qDebug() << direction;
    }
    else if (event->key() == Qt::Key_Down && direction != UP && play_status == PLAYING){
        direction = DOWN;
        //qDebug() << direction;
    }
    else if (event->key() == Qt::Key_Left && direction != RIGHT && play_status == PLAYING){
        direction = LEFT;
        //qDebug() << direction;
    }
    else if (event->key() == Qt::Key_Right && direction != LEFT && play_status == PLAYING){
        direction = RIGHT;
        //qDebug() << direction;
    }
    // quit game
    else if (event->key() == Qt::Key_Escape){
        if (play_status != OVER){
            play_status = OVER;
        }else{
            // close screen
            this->close();
        }

    }else if (event->key() == Qt::Key_Space){
        // pause and resume
        if(play_status == PLAYING){play_status = PAUSE;}
        else if (play_status == PAUSE){play_status = PLAYING;}

        else if (play_status == OVER){
            sque.clear();
            initGame();
        }

    }
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
   // if (play_status == PLAYING || play_status == PAUSE){
        QBrush white(Qt::white,Qt::SolidPattern);
        QRect background(0,0,SIZE*WIDTH, SIZE*HEIGHT);
        painter.fillRect(background, white);

        // draw egg
        QBrush ebrush(Qt::red, Qt::SolidPattern);
        QRect erect(SIZE*egg.x, SIZE*egg.y, SIZE, SIZE);
        painter.fillRect(erect, ebrush);

        // draw snake
        QBrush sbrush(Qt::darkGreen, Qt::SolidPattern);
        for (int i = 0; i < sque.size(); ++i) {
            int tmp_x = sque.at(i)->x;
            int tmp_y = sque.at(i)->y;
            QRect srect(SIZE*tmp_x, SIZE*tmp_y, SIZE, SIZE);
            painter.fillRect(srect, sbrush);
        }
        // draw block
        QBrush bbrush(Qt::black, Qt::SolidPattern);
        QRect brect1(0,0,SIZE,SIZE*(HEIGHT+1));
        QRect brect2(0,0,SIZE*(WIDTH+1),SIZE);
        QRect brect3(SIZE*WIDTH,0,SIZE, SIZE*(HEIGHT+1));
        QRect brect4(0,SIZE*HEIGHT, SIZE*(WIDTH+1),SIZE);
        painter.fillRect(brect1, bbrush);
        painter.fillRect(brect2, bbrush);
        painter.fillRect(brect3, bbrush);
        painter.fillRect(brect4, bbrush);

        /*
        for (int j = 0; j <= WIDTH; ++j){
            QRect brect1(SIZE*j, SIZE*0, SIZE, SIZE);
            QRect brect2(SIZE*j, SIZE*HEIGHT, SIZE, SIZE);
            painter.fillRect(brect1, bbrush);
            painter.fillRect(brect2, bbrush);
        }
        for (int j = 0; j <= HEIGHT; ++j){
            QRect brect3(0, SIZE*j, SIZE, SIZE);
            QRect brect4(SIZE*WIDTH, SIZE*j, SIZE, SIZE);
            painter.fillRect(brect3, bbrush);
            painter.fillRect(brect4, bbrush);
        }
        */

       // draw score
        QString score_v = QString::number(score);
        QString score_text = "Score: ";
        score_text.append(score_v);
        painter.drawText(SIZE, SIZE*(HEIGHT+1) + 2*SIZE, score_text);

        // draw instructions
        QString instruct = "Arrow Key - Move, Space - Pause/Resume, Esc - Quit";
        painter.drawText((WIDTH/2)*SIZE, SIZE*(HEIGHT+1) + 2*SIZE, instruct);

   // }else if (play_status == OVER){ // gameover
     if(play_status == OVER){
        QRect endrect(SIZE*WIDTH/4,SIZE, SIZE*WIDTH/2, (HEIGHT-1)*SIZE);
        QBrush endbrush(Qt::green, Qt::Dense6Pattern);
        painter.fillRect(endrect,endbrush);

        QString score_v = QString::number(score);
        QString score_text = "Game Over! Your Score: ";
        score_text.append(score_v);
        painter.drawText(SIZE*WIDTH/2 - 6*SIZE, SIZE*HEIGHT/2, score_text);

        QString instruct = "Space - Play Again, Esc - Exit";
        painter.drawText(SIZE*WIDTH/2 - 7*SIZE, SIZE*HEIGHT/2 + 3*SIZE, instruct);
    }
}

void Dialog::gameLoop()
{
    if (play_status == PLAYING){
        int tmp_x = sque.back()->x;
        int tmp_y = sque.back()->y;
        switch(direction){
        case UP: enqueue_snake_head(tmp_x, tmp_y - 1); break;
        case DOWN: enqueue_snake_head(tmp_x, tmp_y + 1); break;
        case LEFT: enqueue_snake_head(tmp_x - 1, tmp_y); break;
        case RIGHT: enqueue_snake_head(tmp_x + 1, tmp_y); break;
        }

        // check egg condition
        if(egg_eaten()){
            score++;
            generate_egg();
        }else{
            dequeue_snake_tail();
        }

        // check game over
        if (game_over_check(sque.back()->x, sque.back()->y)){
            play_status = OVER;
        }

        //display_snake_coordinate();

    }

}
