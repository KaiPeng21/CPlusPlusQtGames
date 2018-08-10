#include "game.h"

Game::Game(QWidget *parent)
{
    Q_UNUSED(parent);

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,WIDTH*SIZE,HEIGHT*SIZE);
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(WIDTH*SIZE,HEIGHT*SIZE);

    timer = new QTimer();

    setUp_music();
    setUp_effect();

    background = new Background();
    scene->addItem(background);
    stair_Y_velocity = 50;

    player = new Player();
    scene->addItem(player);

    score = new Score();
    scene->addItem(score);

    honey = new Honey();
    scene->addItem(honey);

    enemy = new Enemy();
    scene->addItem(enemy);

    overscreen = new Overscreen();
    scene->addItem(overscreen);
    scene->addItem(overscreen->overtext);

    initGame();

    connect(timer,SIGNAL(timeout()), player,SLOT(jump_update()));
    connect(timer,SIGNAL(timeout()), player,SLOT(update_pixmap()));
    connect(timer,SIGNAL(timeout()), honey, SLOT(update_honey_pos()));
    connect(timer,SIGNAL(timeout()), player,SLOT(update_player_in_honey_state()));
    connect(timer,SIGNAL(timeout()), enemy, SLOT(update_enemy_pos()));
    connect(timer,SIGNAL(timeout()), enemy, SLOT(update_enemy_pixmap()));
    connect(timer,SIGNAL(timeout()), player,SLOT(keep_player_in_focus()));

    timer->start(50);

    show();
}

void Game::initGame()
{
    game_status = PLAY;

    player->setPos(WIDTH*SIZE/2,HEIGHT*SIZE/3-SIZE);
    player->gravity = 0.5;
    player->velocityY = 0;
    player->onGround = false;
    player->leftMove = false;
    player->rightMove = false;
    player->phs = false;

    stair_Y_velocity = 1;
    for (int i = 0; i < stair_list.size(); i++){
        disconnect_stair_and_timer(stair_list.at(i));
        scene->removeItem(stair_list.at(i));
    }
    stair_list.clear();

    stair_setUp();

    honey->set_honey_appear(false);
    enemy->show_enemy();

    score->reset();
    score->setDefaultTextColor(Qt::darkGreen);

    overscreen->hide_screen();

    musiclist->setCurrentIndex(THM);
    musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    musicplayer->play();
}

void Game::stair_setUp()
{
    Stair * stair = new Stair();
    stair->setPos(WIDTH*SIZE/2-SIZE*3,HEIGHT*SIZE/3);
    scene->addItem(stair);
    add_stair_to_list(stair);

    int low = 0;
    int high = WIDTH*SIZE-4*SIZE;
    int x = qrand() % ((high + 1) - low) + low;

    Stair * ns = new Stair();
    ns->setPos(x,SIZE*2);
    scene->addItem(ns);
    add_stair_to_list(ns);

    int x2 = qrand() % ((high + 1) - low) + low;
    Stair * ns2 = new Stair();
    ns2->setPos(x2,HEIGHT*SIZE*2/3);
    scene->addItem(ns2);
    add_stair_to_list(ns2);

}

void Game::setUp_music()
{
    musicplayer = new QMediaPlayer();
    musiclist = new QMediaPlaylist();
    musiclist->insertMedia(THM, QUrl("qrc:/sounds/mario_theme.mp3"));
    musiclist->insertMedia(STA, QUrl("qrc:/sounds/star.mp3"));
    musiclist->insertMedia(OVE, QUrl("qrc:/sounds/over.mp3"));
    musicplayer->setPlaylist(musiclist);
}

void Game::setUp_effect()
{
    jumpEffect = new QSoundEffect();
    jumpEffect->setSource(QUrl("qrc:/sounds/jump.wav"));
    jumpEffect->setLoopCount(1);
    jumpEffect->setVolume(0.8f);

    ceilEffect = new QSoundEffect();
    ceilEffect->setSource(QUrl("qrc:/sounds/hit_ceil.wav"));
    ceilEffect->setLoopCount(1);
    ceilEffect->setVolume(1.0f);

    bonusEffect = new QSoundEffect();
    bonusEffect->setSource(QUrl("qrc:/sounds/bonus.wav"));
    bonusEffect->setLoopCount(1);
    bonusEffect->setVolume(0.8f);

    kickEffect = new QSoundEffect();
    kickEffect->setSource(QUrl("qrc:/sounds/kick_enemy.wav"));
    kickEffect->setLoopCount(1);
    kickEffect->setVolume(0.8f);
}


void Game::play_effect(int type)
{
    if (type == JMP){
        jumpEffect->play();
    }
    else if (type == BNS){
        bonusEffect->play();
    }
    else if (type == CEL){
        ceilEffect->play();
    }
    else if (type == KIK){
        kickEffect->play();
    }

}

bool Game::game_is_over()
{
    if (game_status == OVER){
        overscreen->show_screen();
        overscreen->setZValue(INT_MAX-1);
        return true;
    }
    if (player->pos().y() > HEIGHT*SIZE || (enemy_touch_player() && !player->phs)){
        game_status = OVER;
        overscreen->show_screen();
        overscreen->setZValue(INT_MAX-1);
        musiclist->setCurrentIndex(OVE);
        musiclist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        musicplayer->play();
        return true;
    }


    overscreen->hide_screen();
    return false;
}

double Game::touch_stair_ground()
{
    for (int i = 0; i < stair_list.size(); i++){
            if (player->pos().y() >= stair_list.at(i)->pos().y() - SIZE
                    && player->pos().y() <= stair_list.at(i)->pos().y()+SIZE/2){
                if (player->pos().x() + SIZE > stair_list.at(i)->pos().x()
                        && player->pos().x() < stair_list.at(i)->pos().x() + 4*SIZE){
                    if (stair_list.at(i)->pos().y() > SIZE*(HEIGHT+2)){
                        scene->removeItem(stair_list.at(i));
                        stair_list.swap(i,stair_list.size()-1);
                        disconnect_stair_and_timer(stair_list.back());
                        stair_list.pop_back();
                    }
                    else{
                        return stair_list.at(i)->pos().y();
                    }
                }
            }
    }

    return -1;
}

bool Game::is_on_ground_offset()
{
    if (player->onGround){
        return true;
    }

    if (!player->phs){
        for (int i = 0; i < stair_list.size(); i++){
                if (player->pos().y() >= stair_list.at(i)->pos().y() - SIZE - stair_Y_velocity - SIZE/5
                        && player->pos().y() <= stair_list.at(i)->pos().y()+SIZE/2+stair_Y_velocity + SIZE/5){
                    if (player->pos().x() + SIZE > stair_list.at(i)->pos().x()
                            && player->pos().x() < stair_list.at(i)->pos().x() + 4*SIZE){
                        if (stair_list.at(i)->pos().y() > SIZE*(HEIGHT+2)){
                            scene->removeItem(stair_list.at(i));
                            stair_list.swap(i,stair_list.size()-1);
                            disconnect_stair_and_timer(stair_list.back());
                            stair_list.pop_back();
                        }
                        else{
                            return true;
                        }
                    }
                }
        }
    }
    else{
        for (int i = 0; i < stair_list.size(); i++){
                if (player->pos().y() >= stair_list.at(i)->pos().y() - SIZE - stair_Y_velocity - SIZE/3
                        && player->pos().y() <= stair_list.at(i)->pos().y()+SIZE/2+stair_Y_velocity + SIZE/3){
                    if (player->pos().x() + SIZE > stair_list.at(i)->pos().x()
                            && player->pos().x() < stair_list.at(i)->pos().x() + 4*SIZE){
                        if (stair_list.at(i)->pos().y() > SIZE*(HEIGHT+2)){
                            scene->removeItem(stair_list.at(i));
                            stair_list.swap(i,stair_list.size()-1);
                            disconnect_stair_and_timer(stair_list.back());
                            stair_list.pop_back();
                        }
                        else{
                            return true;
                        }
                    }
                }
        }
    }

    return false;
}

bool Game::player_hit_stair_ceil()
{
    for (int i = 0; i < stair_list.size(); i++){
            if (player->pos().y() <= stair_list.at(i)->pos().y() + SIZE
                    && player->pos().y() >= stair_list.at(i)->pos().y() && !player->onGround){
                if (player->pos().x() > stair_list.at(i)->pos().x()
                        && player->pos().x() < stair_list.at(i)->pos().x() + 3*SIZE){
                    if (stair_list.at(i)->pos().y() > SIZE*(HEIGHT+2)){
                        scene->removeItem(stair_list.at(i));
                        stair_list.swap(i,stair_list.size()-1);
                        disconnect_stair_and_timer(stair_list.back());
                        stair_list.pop_back();
                    }
                    else{
                        play_effect(CEL);
                        return true;
                    }
                }
            }
    }

    return false;
}

void Game::add_stair_to_list(Stair * st)
{
    stair_list.append(st);
    st->set_st_velocityY(stair_Y_velocity);
    connect(timer,SIGNAL(timeout()), st, SLOT(stair_move()));
}

void Game::disconnect_stair_and_timer(Stair *st)
{
    disconnect(timer, SIGNAL(timeout()), st, SLOT(stair_move()));
}

double Game::honey_touch_stair_ground()
{
    for (int i = 0; i < stair_list.size(); i++){
            if (honey->pos().y() >= stair_list.at(i)->pos().y() - SIZE
                    && honey->pos().y() <= stair_list.at(i)->pos().y()+SIZE/2){
                if (honey->pos().x() + SIZE > stair_list.at(i)->pos().x()
                        && honey->pos().x() < stair_list.at(i)->pos().x() + 4*SIZE){
                    if (stair_list.at(i)->pos().y() > SIZE*(HEIGHT+2)){
                        scene->removeItem(stair_list.at(i));
                        stair_list.swap(i,stair_list.size()-1);
                        disconnect_stair_and_timer(stair_list.back());
                        stair_list.pop_back();
                    }
                    else{
                        return stair_list.at(i)->pos().y();
                    }
                }
            }
    }

    return -1;
}

bool Game::player_get_honey()
{
    if (player->pos().x() == honey->pos().x() && player->pos().y() == honey->pos().y()){
        play_effect(BNS);
        return true;
    }

    if (player->pos().x() <= honey->pos().x()+SIZE && player->pos().x()+SIZE >= honey->pos().x()){
        if (player->pos().y() <= honey->pos().y()+SIZE && player->pos().y()+SIZE >= honey->pos().y()){
            play_effect(BNS);
            return true;
        }
    }

    return false;
}

double Game::enemy_touch_stair_ground()
{
    for (int i = 0; i < stair_list.size(); i++){
            if (enemy->pos().y() >= stair_list.at(i)->pos().y() - SIZE
                    && enemy->pos().y() <= stair_list.at(i)->pos().y()+SIZE/2){
                if (enemy->pos().x() + SIZE > stair_list.at(i)->pos().x()
                        && enemy->pos().x() < stair_list.at(i)->pos().x() + 4*SIZE){
                    if (stair_list.at(i)->pos().y() > SIZE*(HEIGHT+2)){
                        scene->removeItem(stair_list.at(i));
                        stair_list.swap(i,stair_list.size()-1);
                        disconnect_stair_and_timer(stair_list.back());
                        stair_list.pop_back();
                    }
                    else{
                        return stair_list.at(i)->pos().y();
                    }
                }
            }
    }

    return -1;
}

bool Game::enemy_touch_player()
{
    if (!player_jump_on_enemy()){
        if (player->pos().y() >= enemy->pos().y() - SIZE/4
                && player->pos().y() <= enemy->pos().y()+SIZE/4){
            if (player->pos().x() < enemy->pos().x() + SIZE*0.9
                    && player->pos().x() + SIZE*0.6 > enemy->pos().x()){
                return true;
            }
        }
    }
    return false;
}

bool Game::player_jump_on_enemy()
{
    if (enemy_jump_on_player()){
        return false;
    }
    if (is_on_ground_offset()){
        return false;
    }

    if (player->pos().y() >= enemy->pos().y() - SIZE
            && player->pos().y() <= enemy->pos().y()+SIZE/2){
        if (player->pos().x() >= enemy->pos().x() - SIZE*0.9
                && player->pos().x() +SIZE <= enemy->pos().x() + SIZE + SIZE*0.9){
            play_effect(KIK);
            score->increment();
            return true;
        }
    }

    return false;
}

bool Game::enemy_jump_on_player()
{

    if (enemy->pos().y() >= player->pos().y() - SIZE
            && enemy->pos().y() <= player->pos().y()+SIZE/2){
        if (enemy->pos().x() >= player->pos().x() - SIZE*0.7
                && enemy->pos().x() +SIZE <= player->pos().x() + SIZE + SIZE*0.7){
            return true;
        }
    }

    return false;
}


void Game::stair_random_generate()
{

    int low = 0;
    int high = WIDTH*SIZE-4*SIZE;
    int x = qrand() % ((high + 1) - low) + low;

    Stair * ns = new Stair();
    ns->setPos(x,0);
    scene->addItem(ns);
    add_stair_to_list(ns);
}
