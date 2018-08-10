#include "pacmangame.h"

Pacmangame::Pacmangame(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Pacman Game");
    resize((WIDTH)*SIZE, (HEIGHT+1)*SIZE + SIZE);
    initGame();

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
}

Pacmangame::~Pacmangame()
{

}

void Pacmangame::initGame()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    enemy.clear();
    score = 0;
    moves = 0;
    over = false;

    trap.x = trap.y = 0;

    pac.x = WIDTH/2;
    pac.y = HEIGHT/2;

    initMatrix();


    Generate_Item();

    Generate_Enemy();
    Generate_Enemy();
    Generate_Enemy();

    trapCt = 0;
    setTrap();
}

void Pacmangame::initMatrix()
{
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            block[i][j] = false;
            if (i == 0 || i == WIDTH-1 || j == 0 || j == HEIGHT-1){
                block[i][j] = true;
            }
            else if (isPacMan(i,j)){
                block[i][j] = false;
            }
            else{
                block[i][j] = random();
            }
        }
    }

    checkBoundary();
}

bool Pacmangame::random()
{
    bool factor = (bool) (qrand() % 4);
    return !factor;
}

void Pacmangame::Generate_Enemy()
{
    int x, y;
    random();
    do{
        x = qrand() % WIDTH;
        y = qrand() % HEIGHT;
    }while(isPacMan(x,y) || isEnemy(x,y) || isBlock(x,y) || isItem(x,y) || isTrap(x,y));

    addEnemy(x,y);
}

void Pacmangame::addEnemy(int x, int y)
{
    Coordinate * e = new Coordinate();
    e->x = x;
    e->y = y;
    e->weight = Manhatton(x,y);
    enemy.append(e);
}

void Pacmangame::Generate_Item()
{
    random();
    int x, y;
    do{
        x = qrand() % WIDTH;
        y = qrand() % HEIGHT;
    }while(isPacMan(x,y) || isBlock(x,y) || isEnemy(x,y) || isTrap(x,y));
    item.x = x;
    item.y = y;
}


bool Pacmangame::isPacMan(int x, int y)
{
    if (x == pac.x && y == pac.y){
        return true;
    }
    return false;
}

bool Pacmangame::isEnemy(int x, int y)
{
    for (int i = 0; i < enemy.size(); i++){
        if (enemy.at(i)->x == x && enemy.at(i)->y == y){
            return true;
        }
    }
    return false;
}

bool Pacmangame::isBlock(int x, int y)
{
    if (block[x][y]){
        return true;
    }
    return false;
}

bool Pacmangame::isItem(int x, int y)
{
    if (x == item.x && y == item.y){
        return true;
    }
    return false;
}

void Pacmangame::gameOverCheck()
{
    if (isEnemy(pac.x,pac.y)){
        over = true;
    }
}

void Pacmangame::getItemCheck()
{
    if (isItem(pac.x, pac.y)){
        score++;
        Generate_Item();
        Generate_Enemy();
        removeNeighborEnemy();
    }
}

void Pacmangame::moveLeft()
{
    if (!isBlock(pac.x-1,pac.y)){
        pac.x--;
        moves++;
        gameOverCheck();
        if (!over){
            moveEnemies();
            gameOverCheck();
            getItemCheck();
            setTrap();
        }
    }
}

void Pacmangame::moveRight()
{
    if (!isBlock(pac.x+1,pac.y)){
        pac.x++;
        moves++;
        gameOverCheck();
        if (!over){
            moveEnemies();
            gameOverCheck();
            getItemCheck();
            setTrap();
        }
    }
}

void Pacmangame::moveUp()
{
    if (!isBlock(pac.x,pac.y-1)){
        pac.y--;
        moves++;
        gameOverCheck();
        if (!over){
            moveEnemies();
            gameOverCheck();
            getItemCheck();
            setTrap();
        }
    }
}

void Pacmangame::moveDown()
{
    if (!isBlock(pac.x,pac.y+1)){
        pac.y++;
        moves++;
        gameOverCheck();
        if (!over){
            moveEnemies();
            gameOverCheck();
            getItemCheck();
            setTrap();
        }
    }
}

int Pacmangame::Manhatton(int x, int y)
{
    return abs(x - pac.x) + abs(y - pac.y);
}

void Pacmangame::moveEnemies()
{
    for (int i = 0; i < enemy.size(); i++){
        int L = INT_MAX, R = INT_MAX, U = INT_MAX, D = INT_MAX;
        if (!isBlock(enemy.at(i)->x - 1, enemy.at(i)->y)  && !isItem(enemy.at(i)->x - 1, enemy.at(i)->y)){
            L = enemy.at(i)->weight + 1 + Manhatton(enemy.at(i)->x - 1, enemy.at(i)->y);
        }
        if (!isBlock(enemy.at(i)->x + 1, enemy.at(i)->y) && !isItem(enemy.at(i)->x + 1, enemy.at(i)->y)){
            R = enemy.at(i)->weight + 1 + Manhatton(enemy.at(i)->x + 1, enemy.at(i)->y);
        }
        if (!isBlock(enemy.at(i)->x, enemy.at(i)->y - 1) && !isItem(enemy.at(i)->x, enemy.at(i)->y - 1)){
            U = enemy.at(i)->weight + 1 + Manhatton(enemy.at(i)->x, enemy.at(i)->y - 1);
        }
        if (!isBlock(enemy.at(i)->x, enemy.at(i)->y + 1) && !isItem(enemy.at(i)->x, enemy.at(i)->y + 1)){
            D = enemy.at(i)->weight + 1 + Manhatton(enemy.at(i)->x, enemy.at(i)->y + 1);
        }
        int mini = qMin(qMin(L,R),qMin(U,D));
        if (mini != INT_MAX){
            if (mini == L){
                --(enemy.at(i)->x);
            }
            else if (mini == R){
                ++(enemy.at(i)->x);
            }
            else if (mini == U){
                --(enemy.at(i)->y);
            }
            else if (mini == D){
                ++(enemy.at(i)->y);
            }
        }
    }

    enemyTrapCheck();
}

void Pacmangame::removeNeighborEnemy()
{
    for (int i = 0; i < enemy.size(); i++){
        if (isPacNeighbor(enemy.at(i)->x, enemy.at(i)->y)){
            enemy.swap(i, enemy.size()-1);
            enemy.removeLast();
        }
    }
}

bool Pacmangame::isPacNeighbor(int x, int y)
{
    if (x == pac.x+1 && y == pac.y){
        return true;
    }
    if (x == pac.x-1 && y == pac.y){
        return true;
    }
    if (x == pac.x && y == pac.y-1){
        return true;
    }
    if (x == pac.x && y == pac.y+1){
        return true;
    }
    return false;
}

void Pacmangame::checkBoundary()
{
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            boundary[i][j] = 0;
        }
    }

    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            if (!isBlock(i,j)){
                dfs(i,j,1);
                break;
            }
        }
    }

    for (int i = 1; i < WIDTH-1; i++){
        for (int j = 1; j < HEIGHT-1; j++){
            if (!isBlock(i,j) && boundary[i][j] == 0){
                if (i < WIDTH/2){
                    dfs(i,j,1);
                    breakRight(i,j);
                }
                else{
                    dfs(i,j,1);
                    breakLeft(i,j);
                }
            }
        }
    }

}

void Pacmangame::dfs(int x, int y, int ct)
{
    if (isBlock(x,y)){
        return;
    }
    boundary[x][y] = ct;

    if (!isBlock(x-1,y) && boundary[x-1][y] == 0){
        dfs(x-1,y,ct);
    }
    if (!isBlock(x+1,y) && boundary[x+1][y] == 0){
        dfs(x+1,y,ct);
    }
    if (!isBlock(x,y-1) && boundary[x][y-1] == 0){
        dfs(x,y-1,ct);
    }
    if (!isBlock(x,y+1) && boundary[x][y+1] == 0){
        dfs(x,y+1,ct);
    }
}

void Pacmangame::breakRight(int x, int y)
{
    int i = x, j = y;
    while(!isBlock(i,j)){
        i++;
    }
    while(isBlock(i,j) && i < WIDTH-1){
        block[i][j] = false;
        boundary[i][j] = 1;
        i++;
    }
}

void Pacmangame::breakLeft(int x, int y)
{
    int i = x, j = y;
    while(!isBlock(i,j)){
        i--;
    }
    while(isBlock(i,j) && i > 0){
        block[i][j] = false;
        boundary[i][j] = 1;
        i--;
    }
}

void Pacmangame::setTrap()
{
    if (trapCt >= 10){
        trapCt = 0;
        int x, y;
        random();
        do{
            x = qrand() % WIDTH;
          y = qrand() % HEIGHT;
        }while(isPacMan(x,y) || isEnemy(x,y) || isBlock(x,y) || isItem(x,y));
        trap.x = x;
        trap.y = y;
    }
    else{
        trapCt++;
    }
}

bool Pacmangame::isTrap(int x, int y)
{
    if (x == trap.x && y == trap.y){
        return true;
    }
    return false;
}

void Pacmangame::enemyTrapCheck()
{
    for (int i = 0; i < enemy.size(); i++){
        if (isTrap(enemy.at(i)->x, enemy.at(i)->y)){
            enemy.swap(i, enemy.size()-1);
            enemy.removeLast();
        }
    }
}

void Pacmangame::keyPressEvent(QKeyEvent *e)
{
    if (over){
        if (e->key() == Qt::Key_Space){
            initGame();
        }
        else if (e->key() == Qt::Key_Escape){
            this->close();
        }
    }
    if (!over){
        if (e->key() == Qt::Key_Left){
            moveLeft();
        }
        if (e->key() == Qt::Key_Right){
            moveRight();
        }
        if (e->key() == Qt::Key_Up){
            moveUp();
        }
        if (e->key() == Qt::Key_Down){
            moveDown();
        }
        if (e->key() == Qt::Key_Escape){
            over = true;
        }
    }

}

void Pacmangame::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);


    QPainter p(this);
    QBrush black(Qt::black, Qt::SolidPattern);
    QBrush red(Qt::red, Qt::SolidPattern);
    QBrush yellow(Qt::yellow, Qt::SolidPattern);
    QBrush cyan(Qt::cyan, Qt::SolidPattern);
    QBrush white(Qt::white, Qt::SolidPattern);
    QBrush gray(Qt::gray, Qt::SolidPattern);
    QBrush trapcolor(Qt::gray, Qt::Dense7Pattern);
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            QRect rect(i*SIZE + 1, j*SIZE + 1, SIZE - 2,  SIZE - 2);
            QRect bod(i*SIZE, j*SIZE, SIZE, SIZE);
            if (isBlock(i,j)){
                p.fillRect(bod,black);
                p.fillRect(rect,gray);
            }
            else if (isPacMan(i,j)){
                p.fillRect(bod,black);
                p.fillRect(rect,cyan);
            }
            else if (isItem(i,j)){
                p.fillRect(bod,black);
                p.fillRect(rect,yellow);
            }
            else if (isEnemy(i,j)){
                p.fillRect(bod,black);
                p.fillRect(rect, red);
            }
            else if (isTrap(i,j)){
                p.fillRect(rect, trapcolor);
            }
            else{
                p.fillRect(bod, white);
                p.fillRect(rect, white);
            }
        }
    }

    p.drawText(WIDTH*SIZE/2-SIZE*3,HEIGHT*SIZE+SIZE, "MOVES: " + QString::number(moves) + "  \tSCORE: " + QString::number(score));

    if (over){
        QBrush overcolor(Qt::red, Qt::Dense6Pattern);
        QRect overscreen(SIZE,HEIGHT*SIZE/4, (WIDTH-2)*SIZE, HEIGHT*SIZE/2);
        p.fillRect(overscreen, overcolor);
        p.drawText(WIDTH*SIZE/2-SIZE*2,HEIGHT*SIZE/2, "Game Over!");
    }

}
