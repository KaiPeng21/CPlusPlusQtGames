#ifndef GAME_H
#define GAME_H

#include "constant.h"
#include "player.h"
#include "background.h"
#include "stair.h"
#include "honey.h"
#include "enemy.h"
#include "score.h"
#include "overscreen.h"

class Game: public QGraphicsView{
public:
    Game(QWidget *parent = 0);

    QGraphicsScene * scene;
    QTimer * timer;
    QTimer * stdown_timer;
    int game_status;
    Background * background;
    Score * score;
    Overscreen * overscreen;
    Player * player;
    QList <Stair *> stair_list;
    double stair_Y_velocity;
    Honey * honey;
    Enemy * enemy;

    QMediaPlaylist * musiclist;
    QMediaPlayer * musicplayer;

    QSoundEffect * jumpEffect;
    QSoundEffect * bonusEffect;
    QSoundEffect * ceilEffect;
    QSoundEffect * kickEffect;

    void initGame();
    void stair_setUp();
    void setUp_music();
    void setUp_effect();
    void play_effect(int type);
    bool game_is_over();
    double touch_stair_ground();
    bool is_on_ground_offset();
    bool player_hit_stair_ceil();

    void add_stair_to_list(Stair * st);
    void disconnect_stair_and_timer(Stair * st);

    double honey_touch_stair_ground();
    bool player_get_honey();

    double enemy_touch_stair_ground();
    bool enemy_touch_player();
    bool player_jump_on_enemy();
    bool enemy_jump_on_player();


public slots:
    void stair_random_generate();

private:
    double timing;
};


#endif // GAME_H
