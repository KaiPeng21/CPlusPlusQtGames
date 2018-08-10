#include "snakeautos.h"

SnakeAutoS::SnakeAutoS(QWidget *parent) :
    QDialog(parent)
{
    //initGame();

    auto_initGame();


    QTimer * timer = new QTimer(this);
    // update game data
    //connect(timer,SIGNAL(timeout()),this,SLOT(game_loop()));
    connect(timer, SIGNAL(timeout()), this, SLOT(auto_loop()));

    // update graphical display
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(20);
}

SnakeAutoS::~SnakeAutoS()
{
}

void SnakeAutoS::initGame()
{
    setWindowTitle("Snake Game");
    resize(WIDTH*SIZE, HEIGHT*SIZE + SIZE);

    play_status = PLAYING;
    direction = UP;
    score = 0;

    sque.clear();

    for (int i = 0; i < 3; i++){
        Snake_Enqueue(WIDTH/2, HEIGHT/2+2-i);
    }

    Generate_Egg();
}

int SnakeAutoS::get_next_x()
{
    int curr_x = sque.back()->x;
    if (direction == UP || direction == DOWN){
        return curr_x;
    }
    if (direction == LEFT){
        return curr_x - 1;
    }
    return curr_x + 1;
}

int SnakeAutoS::get_next_y()
{
    int curr_y = sque.back()->y;
    if (direction == LEFT || direction == RIGHT){
        return curr_y;
    }
    if (direction == UP){
        return curr_y - 1;
    }
    return curr_y + 1;
}

void SnakeAutoS::auto_initGame()
{
    setWindowTitle("AI Snake");
    resize(WIDTH*SIZE, HEIGHT*SIZE + SIZE);

    play_status = AUTO;
    direction = UP;
    score = 0;

    sque.clear();

    for (int i = 0; i < 3; i++){
        Snake_Enqueue(WIDTH/2, HEIGHT/2+2-i);
    }

    Generate_Egg();

    init_Matrix();
    Path.clear();
    BFS();
}

void SnakeAutoS::Snake_Enqueue(int x, int y)
{
    Coordinate * s = new Coordinate();
    s->x = x;
    s->y = y;
    sque.enqueue(s);
}

void SnakeAutoS::Snake_Dequeue()
{
    if (!sque.empty()){
        Coordinate *tmp = sque.dequeue();
        delete tmp;
    }
}

void SnakeAutoS::Path_Enqueue(int x, int y)
{
    Coordinate * p = new Coordinate();
    p->x = x;
    p->y = y;
    Path.enqueue(p);
}

void SnakeAutoS::Path_Dequeue()
{
    if (!Path.empty()){
        Coordinate *tmp = Path.dequeue();
        delete tmp;
    }
}

void SnakeAutoS::setPrev(int x, int y, int px, int py)
{
    Coordinate p;
    p.x = px;
    p.y = py;
    Prev[x][y] = p;
}

void SnakeAutoS::Generate_Egg()
{
    QTime now = QTime::currentTime();
    qsrand(now.msec());

    int egg_x, egg_y;
    do{
        egg_x = qrand() % WIDTH;
        egg_y = qrand() % HEIGHT;
    }while(is_snake(egg_x, egg_y) || is_block(egg_x, egg_y));

    egg.x = egg_x;
    egg.y = egg_y;
}

bool SnakeAutoS::is_block(int x, int y)
{
    if(x == 0 || x == WIDTH-1){
        return true;
    }
    if (y == 0 || y == HEIGHT-1){
        return true;
    }
    return false;
}

bool SnakeAutoS::is_snake_body(int x, int y)
{   
    for (int i = 0; i < sque.size()-1; i++){
        if (sque.at(i)->x == x && sque.at(i)->y == y){
            return true;
        }
    }

    return false;
}

bool SnakeAutoS::is_snake(int x, int y)
{
    if (is_snake_body(x, y) || (sque.back()->x == x && sque.back()->y == y)){
        return true;
    }
    return false;
}

bool SnakeAutoS::is_egg(int x, int y)
{
    if (x == egg.x && y == egg.y){
        return true;
    }
    return false;
}

bool SnakeAutoS::egg_eaten()
{
    if (sque.back()->x == egg.x && sque.back()->y == egg.y){
        return true;
    }
    return false;
}

bool SnakeAutoS::game_over()
{
    if (is_snake_body(sque.back()->x, sque.back()->y) || is_block(sque.back()->x, sque.back()->y)){
        return true;
    }
    return false;
}

bool SnakeAutoS::is_snake_head(int x, int y)
{
    if (x == sque.back()->x && y == sque.back()->y){
        return true;
    }
    return false;
}

void SnakeAutoS::init_Matrix()
{
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            if (is_snake_body(i, j) || is_block(i, j)){
                Weight[i][j] = INT_MAX;
            }
            else if (is_egg(i, j)){
                Weight[i][j] = 0;
            }
            else{
                Weight[i][j] = INT_MAX - 1;
            }
            Visited[i][j] = false;
            Prev[i][j].x = 0;
            Prev[i][j].y = 0;
        }
    }
}

void SnakeAutoS::BFS()
{
    Path_Enqueue(egg.x,egg.y);
    while(!Path.empty()){
        Coordinate * v = Path.front();
        Path_Dequeue();
        Visited[v->x][v->y] = true;
        int W = Weight[v->x][v->y] + 1;

        if (!Visited[v->x - 1][v->y] && Weight[v->x - 1][v->y] != INT_MAX && W < Weight[v->x - 1][v->y]){
            Weight[v->x - 1][v->y] = W;
            setPrev(v->x-1,v->y,v->x,v->y);
            Path_Enqueue(v->x - 1, v->y);
            if (is_snake_head(v->x-1, v->y)){
                break;
            }
        }
        if (!Visited[v->x + 1][v->y] && Weight[v->x + 1][v->y] != INT_MAX && W < Weight[v->x + 1][v->y]){
            Weight[v->x + 1][v->y] = W;
            setPrev(v->x+1,v->y,v->x,v->y);
            Path_Enqueue(v->x + 1, v->y);
            if (is_snake_head(v->x+1, v->y)){
                break;
            }
        }
        if (!Visited[v->x][v->y - 1] && Weight[v->x][v->y - 1] != INT_MAX && W < Weight[v->x][v->y - 1]){
            Weight[v->x][v->y - 1] = W;
            setPrev(v->x,v->y-1,v->x,v->y);
            Path_Enqueue(v->x, v->y - 1);
            if (is_snake_head(v->x, v->y-1)){
                break;
            }
        }
        if (!Visited[v->x][v->y + 1] && Weight[v->x][v->y + 1] != INT_MAX && W < Weight[v->x][v->y + 1]){
            Weight[v->x][v->y + 1] = W;
            setPrev(v->x,v->y+1,v->x,v->y);
            Path_Enqueue(v->x, v->y + 1);
            if (is_snake_head(v->x, v->y+1)){
                break;
            }
        }

    }
}

void SnakeAutoS::game_loop()
{
    if (play_status == PLAYING){
        int next_x = get_next_x();
        int next_y = get_next_y();
        Snake_Enqueue(next_x, next_y);

        if (egg_eaten()){
            Generate_Egg();
            score++;
        }
        else{
            Snake_Dequeue();
        }

        if(game_over()){
            play_status = OVER;
        }
    }

}

void SnakeAutoS::auto_loop()
{
    if (play_status == AUTO){

        int sh_x = sque.back()->x;
        int sh_y = sque.back()->y;

        if (Prev[sh_x][sh_y].x == sh_x-1 && Prev[sh_x][sh_y].y == sh_y){
            direction = LEFT;
        }
        else if (Prev[sh_x][sh_y].x == sh_x+1 && Prev[sh_x][sh_y].y == sh_y){
            direction = RIGHT;
        }
        else if (Prev[sh_x][sh_y].x == sh_x && Prev[sh_x][sh_y].y == sh_y-1){
            direction = UP;
        }
        else if (Prev[sh_x][sh_y].x == sh_x && Prev[sh_x][sh_y].y == sh_y+1){
            direction = DOWN;
        }

        int next_x = get_next_x();
        int next_y = get_next_y();
        Snake_Enqueue(next_x, next_y);

        if (egg_eaten()){
            Generate_Egg();
            score++;

            init_Matrix();
            Path.clear();
            BFS();

        }
        else{
            Snake_Dequeue();
        }

        if(game_over()){
            play_status = OVER;
        }
    }
}



void SnakeAutoS::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);

    QBrush white(Qt::white, Qt::SolidPattern);
    QRect background(SIZE,SIZE,SIZE*(WIDTH-2), SIZE*(HEIGHT-2));
    p.fillRect(background, white);

    QBrush green(Qt::darkGreen, Qt::SolidPattern);
    for (int i = 0; i < sque.size(); i++){
        QRect snake(SIZE*sque.at(i)->x, SIZE*sque.at(i)->y, SIZE, SIZE);
        p.fillRect(snake, green);
    }


    QBrush red(Qt::red, Qt::SolidPattern);
    QRect eg(SIZE*egg.x, SIZE*egg.y, SIZE, SIZE);
    p.fillRect(eg, red);

    QBrush black(Qt::black, Qt::SolidPattern);
    QRect border1(0,0,SIZE, SIZE*HEIGHT);
    QRect border2(0,0,SIZE*WIDTH, SIZE);
    QRect border3(SIZE*(WIDTH-1),0,SIZE, SIZE*HEIGHT);
    QRect border4(0,SIZE*(HEIGHT-1),SIZE*WIDTH, SIZE);
    p.fillRect(border1, black);
    p.fillRect(border2, black);
    p.fillRect(border3, black);
    p.fillRect(border4, black);

    QRect shead(SIZE*sque.back()->x, SIZE*sque.back()->y, SIZE, SIZE);
    QBrush g(Qt::green, Qt::SolidPattern);
    p.fillRect(shead, g);

    p.drawText(WIDTH*SIZE/2-SIZE, HEIGHT*SIZE+SIZE*2/3, "SCORE: " + QString::number(score));

    if (play_status == OVER){
        QRect gameover(SIZE,SIZE*HEIGHT/4,SIZE*(WIDTH-2), SIZE*HEIGHT/2);
        QBrush gg(Qt::cyan, Qt::Dense6Pattern);
        p.fillRect(gameover, gg);
        p.drawText(WIDTH*SIZE/2-SIZE*2, HEIGHT*SIZE/2, "Game Over! Your Score: " + QString::number(score));
    }
}

void SnakeAutoS::keyPressEvent(QKeyEvent *event)
{
    if (play_status == PLAYING){
        if (direction != LEFT && event->key()== Qt::Key_Right){
            direction = RIGHT;
        }
        if (direction != RIGHT && event->key() == Qt::Key_Left){
            direction = LEFT;
        }
        if (direction != UP && event->key() == Qt::Key_Down){
            direction = DOWN;
        }
        if (direction != DOWN && event->key() == Qt::Key_Up){
            direction = UP;
        }
    }

    if (event->key() == Qt::Key_Space){
        if (play_status == OVER){
            //initGame();
            auto_initGame();
        }
        else if (play_status == PLAYING){
            play_status = PAUSE;
        }
        else if (play_status == PAUSE){
            play_status = PLAYING;
        }
    }

    if (event->key() == Qt::Key_Escape){
        if (play_status != OVER){
            play_status = OVER;
        }
        else{
            this->close();
        }
    }
}

