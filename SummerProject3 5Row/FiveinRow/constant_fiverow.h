#ifndef CONSTANT_FIVEROW_H
#define CONSTANT_FIVEROW_H


#define LENGTH 20
#define SIZE 30

#define EMP 0 // empty
#define BLK 1 // black
#define WHT 2 // whtie
#define BOD 3 // border

#define OVER 0
#define PLAY 1
#define AI 2
#define PRE 3

#define NOTEND 0
#define BLKWIN 1
#define WHTWIN 2
#define TIE 3


#define LIVE1 5
#define DEAD3 4
#define LIVE2 3
#define LIVE3 2
#define CONS4 1


/*
#define CONS4 100
#define LIVE3 50
#define LIVE2 40
#define DEAD3 25
#define LIVE1 1
*/

typedef struct _coordinate{
    int x;
    int y;
    int typ;
}Coordinate;

typedef struct _ai_coordinate{
    int x;
    int y;
    int priority;
}AI_Coord;

#endif // CONSTANT_FIVEROW_H
