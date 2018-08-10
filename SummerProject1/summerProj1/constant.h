#ifndef CONSTANT_H
#define CONSTANT_H


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define SIZE 30
#define WIDTH 10
#define HEIGHT 10

#define OVER 0
#define PLAYING 1
#define PAUSE 2
#define AUTO 3

#define BLK 0 // blank
#define EEG 1 // egg
#define SNK 2 // snake
#define BOD 3 // border

typedef struct _Coordinate{
    int x;
    int y;
}Coordinate;

#endif // CONSTANT_H
