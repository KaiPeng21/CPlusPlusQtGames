#include "fiverow.h"

FiveRow::FiveRow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Five in a Row");
    resize(LENGTH*SIZE, LENGTH*SIZE+50);
    initGame();
    connect(this, SIGNAL(clocking()), this, SLOT(update()));


    QMediaPlaylist * musiclist = new QMediaPlaylist;
    musiclist->addMedia(QUrl("qrc:/sounds/ost_sabo_return.mp3"));
    musiclist->addMedia(QUrl("qrc:/sounds/ost_tailmon.mp3"));
    musiclist->addMedia(QUrl("qrc:/sounds/ost_july.mp3"));
    musiclist->addMedia(QUrl("qrc:/sounds/ost_shanks_end_war.mp3"));

    musiclist->setPlaybackMode(QMediaPlaylist::Loop);

    QMediaPlayer * bgmusic = new QMediaPlayer();
    bgmusic->setPlaylist(musiclist);
    bgmusic->play();

}

FiveRow::~FiveRow()
{

}

void FiveRow::initGame()
{
    status = PRE;

    cursor.x = LENGTH/2;
    cursor.y = LENGTH/2;

    turn = true;

    set_matrix();

    moves.clear();

    PQ_DFS.clear();
    PQ_OFS.clear();
}

void FiveRow::set_matrix()
{
    for (int i = 0; i < LENGTH; i++){
        for (int j = 0; j < LENGTH; j++){
            if (i == 0 || i == LENGTH-1 || j == 0 || j == LENGTH-1){
                Type[i][j] = BOD;
            }
            else{
                Type[i][j] = EMP;
            }
        }
    }
}

void FiveRow::moveUp()
{
    if (!is_border(cursor.x, cursor.y-1)){
        --cursor.y;
    }
}

void FiveRow::moveDown()
{
    if (!is_border(cursor.x, cursor.y+1)){
        ++cursor.y;
    }
}

void FiveRow::moveLeft()
{
    if (!is_border(cursor.x-1, cursor.y)){
        --cursor.x;
    }
}

void FiveRow::moveRight()
{
    if (!is_border(cursor.x+1, cursor.y)){
        ++cursor.x;
    }
}

bool FiveRow::is_border(int x, int y)
{
    if (Type[x][y] == BOD){
        return true;
    }
    return false;
}

bool FiveRow::is_black(int x, int y)
{
    if (x <= 0 || x >= LENGTH-1 || y <= 0 || y >= LENGTH-1){
        return false;
    }
    if (Type[x][y] == BLK){
        return true;
    }
    return false;
}

bool FiveRow::is_white(int x, int y)
{
    if (x <= 0 || x >= LENGTH-1 || y <= 0 || y >= LENGTH-1){
        return false;
    }
    if (Type[x][y] == WHT){
        return true;
    }
    return false;
}

bool FiveRow::is_empty(int x, int y)
{
    if (x <= 0 || x >= LENGTH-1 || y <= 0 || y >= LENGTH-1){
        return false;
    }
    if (Type[x][y] == EMP){
        return true;
    }
    return false;
}

bool FiveRow::is_cursor(int x, int y)
{
    if (cursor.x == x && cursor.y == y){
        return true;
    }
    return false;
}

void FiveRow::place()
{

    int x = cursor.x;
    int y = cursor.y;

    if (Type[x][y] != EMP){
        return;
    }
    if(turn){
        Type[x][y] = WHT;
    }
    else{
        Type[x][y] = BLK;
    }

    push_moves(x,y);

    if (!single_check(x,y)){
        turn = !turn;
    }


}

void FiveRow::backMove()
{
    Coordinate *c = pop_moves();
    if (c == nullptr){
        return;
    }

    Type[c->x][c->y] = EMP;
    turn = !turn;

    delete c;
}

void FiveRow::check()
{
    int hori_check = check_horizontal();
    if (hori_check == BLKWIN){
        result = BLKWIN;
        status = OVER;
        return;
    }else if (hori_check == WHTWIN){
        result = WHTWIN;
        status = OVER;
        return;
    }

    int verti_check = check_vertical();
    if (verti_check == BLKWIN){
        result = BLKWIN;
        status = OVER;
        return;
    }else if (verti_check == WHTWIN){
        result = WHTWIN;
        status = OVER;
        return;
    }

    int diag1_check = check_diagonal();
    if (diag1_check == BLKWIN){
        result = BLKWIN;
        status = OVER;
        return;
    }else if (diag1_check == WHTWIN){
        result = WHTWIN;
        status = OVER;
        return;
    }


}

int FiveRow::check_horizontal()
{
    if (turn){ // white
        for (int i = 0; i < LENGTH; i++){
                int ct = 0;
                for (int j = 0; j < LENGTH; j++){
                    if (is_white(i,j)){ct++;}
                    else{ct = 0;}
                    if (ct >= 5){return WHTWIN;}
                }
        }
    }
    else{ // black
        for (int i = 0; i < LENGTH; i++){
                int ct = 0;
                for (int j = 0; j < LENGTH; j++){
                    if (is_black(i,j)){ct++;}
                    else{ct = 0;}
                    if (ct >= 5){return BLKWIN;}
                }
        }
    }

    return NOTEND;
}

int FiveRow::check_vertical()
{
    if (turn){ // white
        for (int i = 0; i < LENGTH; i++){
                int ct = 0;
                for (int j = 0; j < LENGTH; j++){
                    if (is_white(j,i)){ct++;}
                    else{ct = 0;}
                    if (ct >= 5){return WHTWIN;}
                }
        }
    }
    else{ // black
        for (int i = 0; i < LENGTH; i++){
                int ct = 0;
                for (int j = 0; j < LENGTH; j++){
                    if (is_black(j,i)){ct++;}
                    else{ct = 0;}
                    if (ct >= 5){return BLKWIN;}
                }
        }
    }

    return NOTEND;
}

int FiveRow::check_diagonal()
{
    if (turn){ // white
        int j = 0;
        for (int i = 0; i < LENGTH-6; i++){
            int ct = 0;
            int ct2 = 0;
            int ct3 = 0;
            int ct4 = 0;

            for (j = 1; j < LENGTH -1 - i; j++){
                if (is_white(j,j+i)){ct++;}
                else{ct = 0;}
                if (ct >= 5){return WHTWIN;}

                if (is_white(j+i,j)){ct2++;}
                else{ct2 = 0;}
                if (ct2 >= 5){return WHTWIN;}

                if (is_white(LENGTH-1-(i+j),j)){ct3++;}
                else{ct3 = 0;}
                if (ct3 >= 5){return WHTWIN;}

                if (is_white(LENGTH-1-j,i+j)){ct4++;}
                else{ct4 = 0;}
                if (ct4 >= 5){return WHTWIN;}
            }

        }
    }
    else{ // black
        int j = 0;
        for (int i = 0; i < LENGTH-6; i++){
            int ct = 0;
            int ct2 = 0;
            int ct3 = 0;
            int ct4 = 0;

            for (j = 1; j < LENGTH -1 - i; j++){
                if (is_black(j,j+i)){ct++;}
                else{ct = 0;}
                if (ct >= 5){return BLKWIN;}

                if (is_black(j+i,j)){ct2++;}
                else{ct2 = 0;}
                if (ct2 >= 5){return BLKWIN;}

                if (is_black(LENGTH-1-(i+j),j)){ct3++;}
                else{ct3 = 0;}
                if (ct3 >= 5){return BLKWIN;}

                if (is_black(LENGTH-1-j,i+j)){ct4++;}
                else{ct4 = 0;}
                if (ct4 >= 5){return BLKWIN;}
            }
        }
    }
    return NOTEND;
}

bool FiveRow::single_check(int x, int y)
{
    int hori_check = single_horizontal_check(x,y);
    if (hori_check == BLKWIN){
        result = BLKWIN;
        status = OVER;
        return true;
    }else if (hori_check == WHTWIN){
        result = WHTWIN;
        status = OVER;
        return true;
    }

    int verti_check = single_vertical_check(x,y);
    if (verti_check == BLKWIN){
        result = BLKWIN;
        status = OVER;
        return true;
    }else if (verti_check == WHTWIN){
        result = WHTWIN;
        status = OVER;
        return true;
    }

    int diag1_check = check_diagonal();
    if (diag1_check == BLKWIN){
        result = BLKWIN;
        status = OVER;
        return true;
    }else if (diag1_check == WHTWIN){
        result = WHTWIN;
        status = OVER;
        return true;
    }

    if (moves.size() == (LENGTH-2)*(LENGTH-2)){
        result = TIE;
        status = OVER;
        return true;
    }

    return false;
}

int FiveRow::single_horizontal_check(int x, int y)
{
    Q_UNUSED(y);
    if (turn){ // white
        int ct = 0;
        for (int j = 0; j < LENGTH; j++){
            if (is_white(x,j)){ct++;}
            else{ct = 0;}
            if (ct >= 5){return WHTWIN;}
        }
    }
    else{ // black
        int ct = 0;
        for (int j = 0; j < LENGTH; j++){
            if (is_black(x,j)){ct++;}
            else{ct = 0;}
            if (ct >= 5){return BLKWIN;}
        }
    }

    return NOTEND;
}

int FiveRow::single_vertical_check(int x, int y)
{
    Q_UNUSED(x);
    if (turn){ // white
        int ct = 0;
        for (int j = 0; j < LENGTH; j++){
            if (is_white(j,y)){ct++;}
            else{ct = 0;}
            if (ct >= 5){return WHTWIN;}
        }
    }
    else{ // black
        int ct = 0;
        for (int j = 0; j < LENGTH; j++){
            if (is_black(j,y)){ct++;}
            else{ct = 0;}
            if (ct >= 5){return BLKWIN;}
        }
    }

    return NOTEND;
}

void FiveRow::push_moves(int x, int y)
{
    Coordinate * c = new Coordinate();
    c->x = x;
    c->y = y;
    c->typ = turn&WHT || !turn&BLK;
    moves.push_back(c);
}

Coordinate *FiveRow::pop_moves()
{
    if (moves.empty()){
        return nullptr;
    }
    Coordinate * c = moves.back();
    moves.pop_back();
    return c;
}

void FiveRow::PQ_DFS_Enqueue(int x, int y, int p)
{
    /*
    AI_Coord * coord = new AI_Coord();
    coord->x = x;
    coord->y = y;
    coord->priority = p;

    int k = PQ_DFS.indexOf(coord);

    if (k != -1){
        coord->priority = p + PQ_DFS.at(k)->priority;
    }

    int i = 0;
    while (i < PQ_DFS.size() && PQ_DFS.at(i)->priority > coord->priority){
        i++;
    }
    if (i < PQ_DFS.size()){
        PQ_DFS.insert(i,coord);
    }
    else{
        PQ_DFS.push_back(coord);
    }

    */

    AI_Coord * coord = new AI_Coord();
    coord->x = x;
    coord->y = y;
    coord->priority = p;

    int k = PQ_DFS.indexOf(coord);

    // found in PQ, update only if priority is higher (lower number)
    if (k != -1){
        if (PQ_DFS.at(k)->priority <= p){
            delete coord;
            return;
        }
        else{
            PQ_DFS.removeAt(k);
        }
    }

    int i = 0;
    while (i < PQ_DFS.size() && PQ_DFS.at(i)->priority < p){
        i++;
    }
    if (i < PQ_DFS.size()){
        PQ_DFS.insert(i,coord);
    }
    else{
        PQ_DFS.push_back(coord);
    }

}

AI_Coord *FiveRow::PQ_DFS_Dequeue()
{
    if (PQ_DFS.empty()){
        return nullptr;
    }
    AI_Coord * coord = PQ_DFS.front();
    PQ_DFS.pop_front();

    return coord;
}

void FiveRow::PQ_OFS_Enqueue(int x, int y, int p)
{
    /*
    AI_Coord * coord = new AI_Coord();
    coord->x = x;
    coord->y = y;
    coord->priority = p;

    int k = PQ_OFS.indexOf(coord);

    if (k != -1){
        coord->priority = p + PQ_OFS.at(k)->priority;
    }

    int i = 0;
    while (i < PQ_OFS.size() && PQ_OFS.at(i)->priority > coord->priority){
        i++;
    }
    if (i < PQ_OFS.size()){
        PQ_OFS.insert(i,coord);
    }
    else{
        PQ_OFS.push_back(coord);
    }

    */

    AI_Coord * coord = new AI_Coord();
    coord->x = x;
    coord->y = y;
    coord->priority = p;

    int k = PQ_OFS.indexOf(coord);

    // found in PQ, update only if priority is higher (lower number)
    if (k != -1){
        if (PQ_OFS.at(k)->priority <= p){
            delete coord;
            return;
        }
        else{
            PQ_OFS.removeAt(k);
        }
    }

    int i = 0;
    while (i < PQ_OFS.size() && PQ_OFS.at(i)->priority < p){
        i++;
    }
    if (i < PQ_OFS.size()){
        PQ_OFS.insert(i,coord);
    }
    else{
        PQ_OFS.push_back(coord);
    }

}

AI_Coord *FiveRow::PQ_OFS_Dequeue()
{
    if (PQ_OFS.empty()){
        return nullptr;
    }
    AI_Coord * coord = PQ_OFS.front();
    PQ_OFS.pop_front();

    return coord;
}

bool FiveRow::AI_Random()
{
    QTime now = QTime::currentTime();
    qsrand(now.msec());

    return (bool) (qrand() % 2);
}

bool FiveRow::AI_Player_Place()
{
    int x = cursor.x;
    int y = cursor.y;

    if (Type[x][y] != EMP){
        return true;
    }

    Type[x][y] = WHT;
    push_moves(x,y);

    if (!single_check(x,y)){
        turn = !turn;
    }


    return false;
}

void FiveRow::AI_Computer_Place()
{
    AI_Random();


    AI_Coord * C = nullptr;
    int x, y;

    do{
        if (PQ_DFS.empty() && PQ_OFS.empty()){
            do{
                x = qrand() % LENGTH;
                y = qrand() % LENGTH;
          }while(!is_empty(x,y));
        }
        else if (PQ_DFS.empty()&&!PQ_DFS.empty()){
            do{
                if (PQ_OFS.empty()){break;}
                C = PQ_OFS_Dequeue();
                x = C->x;
                y = C->y;
                delete C;
            }while(!PQ_OFS.empty() && !is_empty(C->x,C->y));
        }
        else if (!PQ_DFS.empty()&&PQ_OFS.empty()){
            do{
                if (PQ_DFS.empty()){break;}
                C = PQ_DFS_Dequeue();
                x = C->x;
                y = C->y;
                delete C;
            }while(!PQ_DFS.empty() && !is_empty(C->x,C->y));
        }
        else if (PQ_OFS.first()->priority == CONS4){
            do{
                if (PQ_OFS.empty()){break;}
                C = PQ_OFS_Dequeue();
                x = C->x;
                y = C->y;
                delete C;
            }while(!PQ_OFS.empty() && !is_empty(C->x,C->y));
        }
        else if (PQ_OFS.front()->priority < PQ_DFS.front()->priority){
            do{
                if (PQ_OFS.empty()){break;}
                C = PQ_OFS_Dequeue();
                x = C->x;
                y = C->y;
                delete C;
            }while(!PQ_OFS.empty() && !is_empty(C->x,C->y));
        }
        else if (PQ_DFS.front()->priority <= PQ_DFS.front()->priority){
            do{
                if (PQ_DFS.empty()){break;}
                C = PQ_DFS_Dequeue();
                x = C->x;
                y = C->y;
                delete C;
            }while(!PQ_DFS.empty() && !is_empty(C->x,C->y));
        }

    }while(!is_empty(x,y));


    /*
    // defense_mode
    if (!PQ_DFS.empty()){
        do{
            if (PQ_DFS.empty()){break;}
            C = PQ_DFS_Dequeue();
            x = C->x;
            y = C->y;
            delete C;
        }while(!PQ_DFS.empty() && !is_empty(C->x,C->y));

    }
    // offense_mode
    else{
        do{
            x = qrand() % LENGTH;
            y = qrand() % LENGTH;
        }while(!is_empty(x,y));
    }

    while(!is_empty(x,y)){
        x = qrand() % LENGTH;
        y = qrand() % LENGTH;
    }
    */


    cursor.x = x;
    cursor.y = y;
    Type[x][y] = BLK;

    push_moves(x,y);

    if (!single_check(x,y)){
        turn = !turn;
    }

}

void FiveRow::AI_backMove()
{
    Coordinate *c = pop_moves();
    if (c == nullptr){
        return;
    }

    Type[c->x][c->y] = EMP;
    turn = !turn;

    delete c;

    Coordinate *d = pop_moves();
    if (d == nullptr){
        return;
    }

    AI_ultimate_offense();

    Type[d->x][d->y] = EMP;
    turn = !turn;

    delete d;

    AI_ultimate_defense();
}

void FiveRow::AI_defense()
{
    if (moves.empty()){
        return;
    }
    int x = moves.back()->x;
    int y = moves.back()->y;
    AI_vertical_defense(x,y);
    AI_horizontal_defense(x,y);
    AI_diagonal_defense(x,y);
}

void FiveRow::AI_vertical_defense(int x, int y)
{
    Q_UNUSED(y);

    int ct = 0;
    int ytmp = 1;
    for (int i = 1; i < LENGTH-1; i++){
        if (!is_white(x,i)){ct = 0; ytmp = i;}
        else{
            ct++;

            if (ct == 1 && is_empty(x, i+1)){
                if (is_white(x,i+2) && is_white(x,i+3) && is_white(x,i+4)){
                    PQ_DFS_Enqueue(x,i+1,CONS4);
                }
                else if (is_white(x,i+2) && is_white(x,i+3) && is_empty(x,i+4) && is_empty(x,ytmp)){
                    PQ_DFS_Enqueue(x,i+1,LIVE3);
                }
                else if (is_white(x,i+2) && is_empty(x,i+3) && is_empty(x,ytmp)){
                    PQ_DFS_Enqueue(x,i+1,LIVE2);
                }
                else if (is_empty(x,ytmp)){
                    PQ_DFS_Enqueue(x,i+1,LIVE1);
                }
            }

            if (ct == 2 && is_empty(x,i+1) && is_white(x,i+2) && is_white(x,i+3)){
                PQ_DFS_Enqueue(x,i+1, CONS4);
            }
            else if (ct == 2 && is_empty(x,i+1) && is_white(x,i+2) && is_empty(x,ytmp)){
                PQ_DFS_Enqueue(x,i+1, LIVE3);
            }
            else if (ct == 2 && is_empty(x,i+1) && is_empty(x,ytmp)){
                if (AI_Random()){PQ_DFS_Enqueue(x,i+1,LIVE2);}
                else{            PQ_DFS_Enqueue(x,ytmp,LIVE2);}
            }


            if (ct == 3 && is_empty(x,i+1) && is_white(x,i+2)){
                PQ_DFS_Enqueue(x,i+1,CONS4);
            }
            else if (ct == 3 && is_empty(x,i+1) && is_empty(x,ytmp)){
                if (AI_Random()){PQ_DFS_Enqueue(x,i+1,LIVE3);}
                else{            PQ_DFS_Enqueue(x,ytmp,LIVE3);}
            }


            if (ct == 4 && is_empty(x,i+1)){
                PQ_DFS_Enqueue(x,i+1,CONS4);
            }
            if (ct == 4 && is_empty(x,ytmp)){
                PQ_DFS_Enqueue(x,ytmp, CONS4);
            }


        }
    }



}

void FiveRow::AI_horizontal_defense(int x, int y)
{
    Q_UNUSED(x);

    int ct = 0;
    int xtmp = 1;
    for (int i = 1; i < LENGTH-1; i++){
        if (!is_white(i,y)){ct = 0; xtmp = i;}
        else{
            ct++;

            if (ct == 1 && is_empty(i+1, y)){
                if (is_white(i+2,y) && is_white(i+3,y) && is_white(i+4,y)){
                    PQ_DFS_Enqueue(i+1,y,CONS4);
                }
                else if (is_white(i+2,y) && is_white(i+3,y) && is_empty(i+4,y) && is_empty(xtmp,y)){
                    PQ_DFS_Enqueue(i+1,y,LIVE3);
                }
                else if (is_white(i+2,y) && is_empty(i+3,y) && is_empty(xtmp,y)){
                    PQ_DFS_Enqueue(i+1,y,LIVE2);
                }
                else if (is_empty(xtmp,y)){
                    PQ_DFS_Enqueue(i+1,y,LIVE1);
                }
            }

            if (ct == 2 && is_empty(i+1,y) && is_white(i+2,y) && is_white(i+3,y)){
                PQ_DFS_Enqueue(i+1,y, CONS4);
            }
            else if (ct == 2 && is_empty(i+1,y) && is_white(i+2,y) && is_empty(xtmp,y)){
                PQ_DFS_Enqueue(i+1,y, LIVE3);
            }
            else if (ct == 2 && is_empty(i+1,y) && is_empty(xtmp,y)){
                if (AI_Random()){PQ_DFS_Enqueue(i+1,y,LIVE2);}
                else{            PQ_DFS_Enqueue(xtmp,y,LIVE2);}
            }


            if (ct == 3 && is_empty(i+1,y) && is_white(i+2,y)){
                PQ_DFS_Enqueue(i+1,y, CONS4);
            }
            else if (ct == 3 && is_empty(i+1,y) && is_empty(xtmp,y)){
                if(AI_Random()){ PQ_DFS_Enqueue(i+1, y, LIVE3);}
                else{            PQ_DFS_Enqueue(xtmp, y, LIVE3);}
            }


            if (ct == 4 && is_empty(i+1,y)){
                PQ_DFS_Enqueue(i+1, y, CONS4);;
            }
            if (ct == 4 && is_empty(xtmp,y)){
                PQ_DFS_Enqueue(xtmp,y, CONS4);
            }


        }
    }

}

void FiveRow::AI_diagonal_defense(int x, int y)
{
    int ct = 0;
    int xa = x - qMin(x,y) + 1;
    int ya = y - qMin(x,y) + 1;
    int xtmp = xa;
    int ytmp = ya;

    for (int i = 0; i < LENGTH-1; i++){
        if (!is_white(xa+i,ya+i)){ct = 0; xtmp = xa+i; ytmp = ya+i;}
        else{
            ct++;

            if (ct == 1 && is_empty(xa+i+1, ya+i+1)){
                if (is_white(xa+i+2,ya+i+2) && is_white(xa+i+3,ya+i+3) && is_white(xa+i+4,ya+i+4)){
                    PQ_DFS_Enqueue(xa+i+1,ya+i+1,CONS4);
                }
                else if (is_white(xa+i+2,ya+i+2) && is_white(xa+i+3,ya+i+3) && is_empty(xa+i+4,ya+i+4) && is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya+i+1,LIVE3);
                }
                else if (is_white(xa+i+2,ya+i+2) && is_empty(xa+i+3,ya+i+3) && is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya+i+1,LIVE2);
                }
                else if (is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya+i+1,LIVE1);
                }
            }

            if (ct == 2 && is_empty(xa+i+1,ya+i+1) && is_white(xa+i+2,ya+i+2) && is_white(xa+i+3,ya+i+3)){
                PQ_DFS_Enqueue(xa+i+1,ya+i+1, CONS4);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya+i+1) && is_white(xa+i+2,ya+i+2) && is_empty(xtmp,ytmp)){
                PQ_DFS_Enqueue(xa+i+1,ya+i+1, LIVE3);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya+i+1) && is_empty(xtmp,ytmp)){
                if (AI_Random()){PQ_DFS_Enqueue(xa+i+1,ya+i+1,LIVE2);}
                else{            PQ_DFS_Enqueue(xtmp,ytmp,LIVE2);}
            }

            if (ct == 3 && is_empty(xa+i+1,ya+i+1) && is_white(xa+i+2,ya+i+2)){
                PQ_DFS_Enqueue(xa+i+1,ya+i+1, CONS4);
            }
            else if (ct == 3 && is_empty(xa+i+1,ya+i+1) && is_empty(xtmp,ytmp)){
                if(AI_Random()){ PQ_DFS_Enqueue(xa+i+1, ya+i+1, LIVE3);}
                else{            PQ_DFS_Enqueue(xtmp, ytmp, LIVE3);}
            }


            if (ct == 4 && is_empty(xa+i+1,ya+i+1)){
                PQ_DFS_Enqueue(xa+i+1, ya+i+1, CONS4);;
            }
            if (ct == 4 && is_empty(xtmp,ytmp)){
                PQ_DFS_Enqueue(xtmp,ytmp, CONS4);
            }

        }
    }




    ct = 0;
    xa = 1;
    ya = y+x-1;
    xtmp = xa;
    ytmp = ya;

    for (int i = 0; i < LENGTH-1; i++){
        if (!is_white(xa+i,ya-i)){ct = 0; xtmp = xa+i; ytmp = ya-i;}
        else{
            ct++;

            if (ct == 1 && is_empty(xa+i+1, ya-i-1)){
                if (is_white(xa+i+2,ya-i-2) && is_white(xa+i+3,ya-i-3) && is_white(xa+i+4,ya-i-4)){
                    PQ_DFS_Enqueue(xa+i+1,ya-i-1,CONS4);
                }
                else if (is_white(xa+i+2,ya-i-2) && is_white(xa+i+3,ya-i-3) && is_empty(xa+i+4,ya-i-4) && is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya-i-1,LIVE3);
                }
                else if (is_white(xa+i+2,ya-i-2) && is_empty(xa+i+3,ya-i-3) && is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya-i-1,LIVE2);
                }
                else if (is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya-i-1,LIVE1);
                }
            }

            if (ct == 2 && is_empty(xa+i+1,ya-i-1) && is_white(xa+i+2,ya-i-2) && is_white(xa+i+3,ya-i-3)){
                PQ_DFS_Enqueue(xa+i+1,ya-i-1, CONS4);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya-i-1) && is_white(xa+i+2,ya-i-2) && is_empty(xtmp,ytmp)){
                PQ_DFS_Enqueue(xa+i+1,ya-i-1, LIVE3);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya-i-1) && is_empty(xtmp,ytmp)){
                if (AI_Random()){PQ_DFS_Enqueue(xa+i+1,ya-i-1,LIVE2);}
                else{            PQ_DFS_Enqueue(xtmp,ytmp,LIVE2);}
            }

            if (ct == 3 && is_empty(xa+i+1,ya-i-1) && is_white(xa+i+2,ya-i-2)){
                PQ_DFS_Enqueue(xa+i+1,ya-i-1, CONS4);
            }
            else if (ct == 3 && is_empty(xa+i+1,ya-i-1) && is_empty(xtmp,ytmp)){
                if(AI_Random()){ PQ_DFS_Enqueue(xa+i+1, ya-i-1, LIVE3);}
                else{            PQ_DFS_Enqueue(xtmp, ytmp, LIVE3);}
            }


            if (ct == 4 && is_empty(xa+i+1,ya-i-1)){
                PQ_DFS_Enqueue(xa+i+1, ya-i-1, CONS4);;
            }
            if (ct == 4 && is_empty(xtmp,ytmp)){
                PQ_DFS_Enqueue(xtmp,ytmp, CONS4);
            }

        }
    }


}

void FiveRow::AI_offense()
{
    if (moves.empty()){
        return;
    }
    int x = moves.back()->x;
    int y = moves.back()->y;
    AI_vertical_offense(x,y);
    AI_horizontal_offense(x,y);
    AI_diagonal_offense(x,y);
}

void FiveRow::AI_vertical_offense(int x, int y)
{
    Q_UNUSED(y);

    int ct = 0;
    int ytmp = 1;
    for (int i = 1; i < LENGTH-1; i++){
        if (!is_black(x,i)){ct = 0; ytmp = i;}
        else{
            ct++;

            if (ct == 1 && is_empty(x, i+1)){
                if (is_black(x,i+2) && is_black(x,i+3) && is_black(x,i+4)){
                    PQ_OFS_Enqueue(x,i+1,CONS4);
                }
                else if (is_black(x,i+2) && is_black(x,i+3) && is_empty(x,ytmp)){
                    PQ_OFS_Enqueue(x,i+1,LIVE3);
                }
                else if (is_empty(x,ytmp)){
                    PQ_DFS_Enqueue(x,i+1,LIVE1);
                }
            }

            if (ct == 2 && is_empty(x,i+1) && is_black(x,i+2) && is_black(x,i+3)){
                PQ_OFS_Enqueue(x,i+1, CONS4);
            }
            else if (ct == 2 && is_empty(x,i+1) && is_black(x,i+2) && is_empty(x,ytmp)){
                PQ_OFS_Enqueue(x,i+1, LIVE3);
            }
            else if (ct == 2 && is_empty(x,i+1) && is_empty(x,ytmp)){
                PQ_OFS_Enqueue(x,i+1,LIVE2);
                PQ_OFS_Enqueue(x,ytmp,LIVE2);
            }


            if (ct == 3 && is_empty(x,i+1) && is_black(x,i+2)){
                PQ_OFS_Enqueue(x,i+1,CONS4);
            }
            else if (ct == 3 && is_empty(x,i+1) && is_empty(x,ytmp)){
                PQ_OFS_Enqueue(x,i+1,LIVE3);
                PQ_OFS_Enqueue(x,ytmp,LIVE3);
            }
            else if (ct == 3 && is_empty(x,i+1)){
                PQ_OFS_Enqueue(x,i+1,DEAD3);
            }
            else if (ct == 3 && is_empty(x,ytmp)){
                PQ_OFS_Enqueue(x,ytmp,DEAD3);
            }


            if (ct == 4 && is_empty(x,i+1)){
                PQ_OFS_Enqueue(x,i+1,CONS4);
            }
            if (ct == 4 && is_empty(x,ytmp)){
                PQ_OFS_Enqueue(x,ytmp, CONS4);
            }


        }
    }
}

void FiveRow::AI_horizontal_offense(int x, int y)
{
    Q_UNUSED(x);

    int ct = 0;
    int xtmp = 1;
    for (int i = 1; i < LENGTH-1; i++){
        if (!is_black(i,y)){ct = 0; xtmp = i;}
        else{
            ct++;

            if (ct == 1 && is_empty(i+1, y)){
                if (is_black(i+2,y) && is_black(i+3,y) && is_black(i+4,y)){
                    PQ_OFS_Enqueue(i+1,y,CONS4);
                }
                else if (is_black(i+2,y) && is_black(i+3,y) && is_empty(xtmp,y)){
                    PQ_OFS_Enqueue(i+1,y,LIVE3);
                }
                else if (is_empty(xtmp,y)){
                    PQ_DFS_Enqueue(i+1,y,LIVE1);
                }
            }

            if (ct == 2 && is_empty(i+1,y) && is_black(i+2,y) && is_black(i+3,y)){
                PQ_OFS_Enqueue(i+1,y, CONS4);
            }
            else if (ct == 2 && is_empty(i+1,y) && is_black(i+2,y) && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(i+1,y, LIVE3);
            }
            else if (ct == 2 && is_empty(i+1,y) && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(i+1,y,LIVE2);
                PQ_OFS_Enqueue(xtmp,y,LIVE2);
            }


            if (ct == 3 && is_empty(i+1,y) && is_black(i+2,y)){
                PQ_OFS_Enqueue(i+1,y, CONS4);
            }
            else if (ct == 3 && is_empty(i+1,y) && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(i+1, y, LIVE3);
                PQ_OFS_Enqueue(xtmp, y, LIVE3);
            }
            else if (ct == 3 && is_empty(i+1,y)){
                PQ_OFS_Enqueue(i+1,y,DEAD3);
            }
            else if (ct == 3 && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(xtmp,y,DEAD3);
            }


            if (ct == 4 && is_empty(i+1,y)){
                PQ_OFS_Enqueue(i+1, y, CONS4);
            }
            if (ct == 4 && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(xtmp,y, CONS4);
            }


        }
    }

}

void FiveRow::AI_diagonal_offense(int x, int y)
{
    int ct = 0;
    int xa = x - qMin(x,y) + 1;
    int ya = y - qMin(x,y) + 1;
    int xtmp = xa;
    int ytmp = ya;

    for (int i = 0; i < LENGTH-1; i++){
        if (!is_black(xa+i,ya+i)){ct = 0; xtmp = xa+i; ytmp = ya+i;}
        else{
            ct++;

            if (ct == 1 && is_empty(xa+i+1, ya+i+1)){
                if (is_black(xa+i+2,ya+i+2) && is_black(xa+i+3,ya+i+3) && is_black(xa+i+4,ya+i+4)){
                    PQ_OFS_Enqueue(xa+i+1,ya+i+1,CONS4);
                }
                else if (is_black(xa+i+2,ya+i+2) && is_black(xa+i+3,ya+i+3) && is_empty(xtmp,ytmp)){
                    PQ_OFS_Enqueue(xa+i+1,ya+i+1,LIVE3);
                }
                else if (is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya+i+1,LIVE1);
                }
            }

            if (ct == 2 && is_empty(xa+i+1,ya+i+1) && is_black(xa+i+2,ya+i+2) && is_black(xa+i+3,ya+i+3)){
                PQ_OFS_Enqueue(xa+i+1,ya+i+1, CONS4);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya+i+1) && is_black(xa+i+2,ya+i+2) && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xa+i+1,ya+i+1, LIVE3);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya+i+1) && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xa+i+1,ya+i+1,LIVE2);
                PQ_OFS_Enqueue(xtmp,ytmp,LIVE2);
            }

            if (ct == 3 && is_empty(xa+i+1,ya+i+1) && is_black(xa+i+2,ya+i+2)){
                PQ_OFS_Enqueue(xa+i+1,ya+i+1, CONS4);
            }
            else if (ct == 3 && is_empty(xa+i+1,ya+i+1) && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xa+i+1, ya+i+1, LIVE3);
                PQ_OFS_Enqueue(xtmp, ytmp, LIVE3);
            }
            else if (ct == 3 && is_empty(xa+i+1,ya+i+1)){
                PQ_OFS_Enqueue(xa+i+1,ya+i+1,DEAD3);
            }
            else if (ct == 3 && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(xtmp,ytmp,DEAD3);
            }


            if (ct == 4 && is_empty(xa+i+1,ya+i+1)){
                PQ_OFS_Enqueue(xa+i+1, ya+i+1, CONS4);;
            }
            if (ct == 4 && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xtmp,ytmp, CONS4);
            }

        }
    }




    ct = 0;
    xa = 1;
    ya = y+x-1;
    xtmp = xa;
    ytmp = ya;

    for (int i = 0; i < LENGTH-1; i++){
        if (!is_black(xa+i,ya-i)){ct = 0; xtmp = xa+i; ytmp = ya-i;}
        else{
            ct++;

            if (ct == 1 && is_empty(xa+i+1, ya-i-1)){
                if (is_black(xa+i+2,ya-i-2) && is_black(xa+i+3,ya-i-3) && is_black(xa+i+4,ya-i-4)){
                    PQ_OFS_Enqueue(xa+i+1,ya-i-1,CONS4);
                }
                else if (is_black(xa+i+2,ya-i-2) && is_black(xa+i+3,ya-i-3) && is_empty(xtmp,ytmp)){
                    PQ_OFS_Enqueue(xa+i+1,ya-i-1,LIVE3);
                }
                else if (is_empty(xtmp,ytmp)){
                    PQ_DFS_Enqueue(xa+i+1,ya-i-1,LIVE1);
                }
            }

            if (ct == 2 && is_empty(xa+i+1,ya-i-1) && is_black(xa+i+2,ya-i-2) && is_black(xa+i+3,ya-i-3)){
                PQ_OFS_Enqueue(xa+i+1,ya-i-1, CONS4);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya-i-1) && is_black(xa+i+2,ya-i-2) && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xa+i+1,ya-i-1, LIVE3);
            }
            else if (ct == 2 && is_empty(xa+i+1,ya-i-1) && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xa+i+1,ya-i-1,LIVE2);
                PQ_OFS_Enqueue(xtmp,ytmp,LIVE2);
            }

            if (ct == 3 && is_empty(xa+i+1,ya-i-1) && is_black(xa+i+2,ya-i-2)){
                PQ_OFS_Enqueue(xa+i+1,ya+i+1, CONS4);
            }
            else if (ct == 3 && is_empty(xa+i+1,ya-i-1) && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xa+i+1, ya-i-1, LIVE3);
                PQ_OFS_Enqueue(xtmp, ytmp, LIVE3);
            }
            else if (ct == 3 && is_empty(xa+i+1,ya-i-1)){
                PQ_OFS_Enqueue(xa+i+1,ya-i-1,DEAD3);
            }
            else if (ct == 3 && is_empty(xtmp,y)){
                PQ_OFS_Enqueue(xtmp,ytmp,DEAD3);
            }


            if (ct == 4 && is_empty(xa+i+1,ya-i-1)){
                PQ_OFS_Enqueue(xa+i+1, ya-i-1, CONS4);
            }
            if (ct == 4 && is_empty(xtmp,ytmp)){
                PQ_OFS_Enqueue(xtmp,ytmp, CONS4);
            }


        }
    }

}

void FiveRow::AI_ultimate_defense()
{
    for (int i = 0; i < moves.size(); i=i+2){
        int x = moves.back()->x;
        int y = moves.back()->y;
        AI_vertical_defense(x,y);
        AI_horizontal_defense(x,y);
        AI_diagonal_defense(x,y);
    }

}

void FiveRow::AI_ultimate_offense()
{
    for (int i = 1; i < moves.size(); i=i+2){
        int x = moves.back()->x;
        int y = moves.back()->y;
        AI_vertical_offense(x,y);
        AI_horizontal_offense(x,y);
        AI_diagonal_offense(x,y);
    }
}

void FiveRow::get_help()
{
    QTextEdit * help = new QTextEdit(this);
    help->setWindowFlags(Qt::Dialog);
    help->setReadOnly(true);
    help->setWindowTitle("Help");
    help->resize(200,280);

    QFile file(":/sounds/help.txt");
    QTextStream stream(&file);

    if (!file.open (QIODevice::ReadOnly)){
        qDebug() << "error opening file";
    }

    help->setHtml(stream.readAll());

    file.close();
    help->show();
}

void FiveRow::keyPressEvent(QKeyEvent *e)
{
    if (e->key()){
        clocking();
    }

    if (status != OVER && e->key() == Qt::Key_H){
        get_help();
    }

    if (status == OVER){
        if (e->key()){
            status = PRE;
        }
    }

    if (status == PRE){
        if (e->key() == Qt::Key_X){
            initGame();
            status = PLAY;
        }
        if (e->key() == Qt::Key_Z){
            initGame();
            status = AI;
        }
        if (e->key() == Qt::Key_Escape){
            this->close();
        }
    }
    if (status == PLAY){
        if (e->key() == Qt::Key_Up){
            moveUp();
        }
        if (e->key() == Qt::Key_Down){
            moveDown();
        }
        if (e->key() == Qt::Key_Left){
            moveLeft();
        }
        if (e->key() == Qt::Key_Right){
            moveRight();
        }
        if (e->key() == Qt::Key_Space){
            place();
        }
        if (e->key() == Qt::Key_Escape){
            status = PRE;
        }
        if (e->key() == Qt::Key_Backspace){
            backMove();
        }
    }

    if (status == AI){
        if (e->key() == Qt::Key_Escape){
            status = PRE;
        }
        if (e->key() == Qt::Key_Up){
            moveUp();
        }
        if (e->key() == Qt::Key_Down){
            moveDown();
        }
        if (e->key() == Qt::Key_Left){
            moveLeft();
        }
        if (e->key() == Qt::Key_Right){
            moveRight();
        }
        if (e->key() == Qt::Key_Space){

            bool invalid = AI_Player_Place();
            if (!invalid){
                AI_defense();
                AI_Computer_Place();
                AI_offense();
            }

        }
        if (e->key() == Qt::Key_Escape){
            status = PRE;
        }
        if (e->key() == Qt::Key_Backspace){
            AI_backMove();
        }
    }

}

void FiveRow::mousePressEvent(QMouseEvent *e)
{
    emit clocking();

    if (status == PLAY){
        if (e->button() == Qt::LeftButton){
            if (e->x()/SIZE > 0 && e->x()/SIZE < LENGTH-1 && e->y()/SIZE > 0 && e->y()/SIZE < LENGTH-1){
                cursor.x = e->x()/SIZE;
                cursor.y = e->y()/SIZE;
            }
        }
        if (e->button() == Qt::RightButton && e->x()/SIZE == cursor.x && e->y()/SIZE == cursor.y){
            place();
        }
    }

    if (status == AI){
        if (e->button() == Qt::LeftButton){
            if (e->x()/SIZE > 0 && e->x()/SIZE < LENGTH-1 && e->y()/SIZE > 0 && e->y()/SIZE < LENGTH-1){
                cursor.x = e->x()/SIZE;
                cursor.y = e->y()/SIZE;
            }
        }
        if (e->button() == Qt::RightButton && e->x()/SIZE == cursor.x && e->y()/SIZE == cursor.y){
            bool invalid = AI_Player_Place();
            if (!invalid){
                AI_defense();
                AI_Computer_Place();
                AI_offense();
            }
        }
    }

    if (status == OVER){
        if (e->button() == Qt::LeftButton){
            status = PRE;
        }
    }


    if (e->button() == Qt::LeftButton){
        if (e->x() > SIZE*14 && e->y() > LENGTH*SIZE+SIZE/2
                && e->x() < SIZE*14+SIZE*4 && e->y() < LENGTH*SIZE+SIZE/2+SIZE){
            undo_slot();
        }
        if (e->x() > SIZE*2 && e->y() > LENGTH*SIZE+SIZE/2
                && e->x() < SIZE*2+SIZE*4 && e->y() < LENGTH*SIZE+SIZE/2+SIZE){
            if (status != PRE){status = PRE;}
            else{this->close();}
        }
        if (e->x() > SIZE*8 && e->y() > LENGTH*SIZE+SIZE/2
                && e->x() < SIZE*8+SIZE*4 && e->y() < LENGTH*SIZE+SIZE/2+SIZE){
            get_help();
        }
    }

    if (status == PRE && e->button() == Qt::LeftButton){
        if (e->x() > SIZE*8 && e->y() > LENGTH*SIZE/4+5*SIZE
                && e->x() < SIZE*8+SIZE*4 && e->y() < LENGTH*SIZE/4+5*SIZE+SIZE){
            initGame();
            status = AI;
        }
        if (e->x() > SIZE*8 && e->y() > LENGTH*SIZE/4+7*SIZE
                && e->x() < SIZE*8+SIZE*4 && e->y() < LENGTH*SIZE/4+7*SIZE+SIZE){
            initGame();
            status = PLAY;
        }
    }


}



void FiveRow::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(225,225,160,225));
    this->setPalette(Pal);

    QPainter p(this);
    QBrush black(Qt::black, Qt::SolidPattern);
    QBrush white(Qt::white, Qt::SolidPattern);
    QBrush ligray(QColor(225,225,160,225), Qt::SolidPattern);
    QBrush cursBlack(Qt::black, Qt::Dense2Pattern);
    QBrush cursWhite(Qt::white, Qt::Dense2Pattern);
    QBrush backgroundcolor(QColor(240,212,135,255), Qt::SolidPattern);
    QBrush introbackground(QColor(240,212,135,255), Qt::SolidPattern);
    QBrush introblack(QColor(0,0,0,200), Qt::SolidPattern);

    p.setFont(QFont("Times",12,100,false));
    p.setPen(Qt::white);

    QRect rect_quit(SIZE*2, LENGTH*SIZE+SIZE/2, SIZE*4, SIZE);
    QPixmap map_quit(":/sounds/push.png");
    p.drawPixmap(rect_quit, map_quit);
    p.drawText(rect_quit, Qt::AlignCenter, "Quit");

    QRect rect_help(SIZE*8, LENGTH*SIZE+SIZE/2, SIZE*4, SIZE);
    QPixmap map_help(":/sounds/push.png");
    p.drawPixmap(rect_help, map_help);
    p.drawText(rect_help, Qt::AlignCenter, "Help");

    QRect rect_undo(SIZE*14, LENGTH*SIZE+SIZE/2, SIZE*4, SIZE);
    QPixmap map_undo(":/sounds/push.png");
    p.drawPixmap(rect_undo, map_undo);
    p.drawText(rect_undo, Qt::AlignCenter, "Undo");

    p.setPen(Qt::black);
    if (status == PRE){
        p.fillRect(QRect(0,0,LENGTH*SIZE, LENGTH*SIZE), white);
        for (int i = 0; i < LENGTH; i++){
            for (int j = 0; j < LENGTH; j++){
                QRect edge(i*SIZE,j*SIZE,SIZE,SIZE);
                QRect rect(i*SIZE+1,j*SIZE+1,SIZE-2,SIZE-2);
                p.fillRect(edge,introblack);
                if (!is_border(i,j)){
                    p.fillRect(rect, introbackground);
                }
            }
        }

        QRect intro(SIZE*LENGTH/4,SIZE*LENGTH/4,SIZE*LENGTH/2,SIZE*LENGTH/2);
        p.setPen(Qt::black);
        p.drawRect(intro);
        p.fillRect(intro,ligray);
        p.setFont(QFont("Helvetica", 28, 100, false));
        p.setPen(Qt::darkGreen);
        p.drawText(QRect(LENGTH*SIZE/4+SIZE,LENGTH*SIZE/4+SIZE*2,LENGTH*SIZE/2,2*SIZE), Qt::TextSingleLine, "FIVE in a ROW");


        p.setFont(QFont("Times",12,100,false));
        p.setPen(Qt::white);
        QRect rect_single(SIZE*8,LENGTH*SIZE/4+5*SIZE,SIZE*4,SIZE);
        QPixmap map_single(":/sounds/push.png");
        p.drawPixmap(rect_single, map_single);
        p.drawText(rect_single, Qt::AlignCenter, "Single Player");

        QRect rect_double(SIZE*8,LENGTH*SIZE/4+7*SIZE,SIZE*4,SIZE);
        QPixmap map_double(":/sounds/push.png");
        p.drawPixmap(rect_double, map_double);
        p.drawText(rect_double, Qt::AlignCenter, "Two Players");

        p.setPen(Qt::black);
    }

    if (status != PRE){
        for (int i = 0; i < LENGTH; i++){
            for (int j = 0; j < LENGTH; j++){
                QRect edge(i*SIZE,j*SIZE,SIZE,SIZE);
                QRect rect(i*SIZE+1,j*SIZE+1,SIZE-2,SIZE-2);
                p.fillRect(edge,black);
                if (!is_border(i,j)){
                    p.fillRect(rect, backgroundcolor);
                }
                if (is_cursor(i,j) && turn){
                    QRect curs(i*SIZE, j*SIZE, SIZE, SIZE);
                    p.fillRect(curs,cursWhite);
                }
                if (is_cursor(i,j) && !turn){
                    QRect curs(i*SIZE, j*SIZE, SIZE, SIZE);
                    p.fillRect(curs,cursBlack);
                }
                if (is_black(i,j)){
                    p.setBrush(black);
                    p.drawEllipse(rect);
                }
                if (is_white(i,j)){
                    p.setBrush(white);
                    p.drawEllipse(rect);
                }
            }
        }


    }
    if (status == OVER){
        p.setFont(QFont("times", 40, -1, false));
        if (result == BLKWIN){
            QRect overscreen(SIZE,SIZE*LENGTH/4,SIZE*(LENGTH-2), SIZE*LENGTH/2);
            QBrush overcolor(QColor(225,225,225,200), Qt::SolidPattern);
            p.fillRect(overscreen,overcolor);
            p.setPen(Qt::black);
            p.drawText(QRect(SIZE*LENGTH/2-160, SIZE*LENGTH/2-SIZE, SIZE*LENGTH, 2*SIZE), Qt::TextSingleLine, "BLACK WINS!!");
        }
        else if (result == WHTWIN){
            QRect overscreen(SIZE,SIZE*LENGTH/4,SIZE*(LENGTH-2), SIZE*LENGTH/2);
            QBrush overcolor(QColor(115,115,115,200), Qt::SolidPattern);
            p.fillRect(overscreen,overcolor);
            p.setPen(Qt::white);
            p.drawText(QRect(SIZE*LENGTH/2-160, SIZE*LENGTH/2-SIZE, SIZE*LENGTH, 2*SIZE), Qt::TextSingleLine, "WHITE WINS!!");
        }
        else{
            QRect overscreen(SIZE,SIZE*LENGTH/4,SIZE*(LENGTH-2), SIZE*LENGTH/2);
            p.fillRect(overscreen,ligray);
            p.setPen(Qt::gray);
            p.drawText(QRect(SIZE*LENGTH/2-160, SIZE*LENGTH/2-SIZE, SIZE*LENGTH, 2*SIZE), Qt::TextSingleLine, "GAME  TIES!!");
        }

       // "qDebug() << ""result: " "+ QString(QString::number(result));
    }

}

void FiveRow::undo_slot()
{
    if (status == PLAY){
        backMove();
    }
    else if (status == AI){
        AI_backMove();
    }

}
