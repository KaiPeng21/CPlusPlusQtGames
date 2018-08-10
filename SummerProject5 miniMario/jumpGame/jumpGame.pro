#-------------------------------------------------
#
# Project created by QtCreator 2016-05-22T15:50:58
#
#-------------------------------------------------

QT       += core gui \
         multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jumpGame
TEMPLATE = app


SOURCES += main.cpp \
    game.cpp \
    player.cpp \
    background.cpp \
    stair.cpp \
    score.cpp \
    honey.cpp \
    enemy.cpp \
    overscreen.cpp

HEADERS  += \
    constant.h \
    game.h \
    player.h \
    background.h \
    stair.h \
    score.h \
    honey.h \
    enemy.h \
    overscreen.h

RESOURCES += \
    res.qrc
